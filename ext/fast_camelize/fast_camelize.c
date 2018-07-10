#include <ruby.h>
#include <ruby/encoding.h>
#include <stdlib.h>

typedef struct current_character {
  unsigned int value;

  int size;
} current_character_t;

typedef struct output_builder {
  enum state {
    START,
    LAST_CHARACTER_UNDERSCORE,
    LAST_CHARACTER_TEXT
  } current_state;

  char *result;
  long result_size;

} output_builder_t;

/* static void */
/* builder_result_push_char(builder_t *builder, unsigned int character, int size, */
                         /* rb_encoding *encoding) { */
  /* if (character_is_upper(character)) { */
    /* if (builder->pushNext == 1) { */
      /* builder->pushNext = 0; */
      /* builder_result_push_literal(builder, '_'); */
    /* } */

    /* builder->result[builder->result_size++] = (char) character - 'A' + 'a'; */
    /* return; */
  /* } */

  /* builder->pushNext = (character_is_lower(character) || character_is_digit(character)); */

  /* if (encoding == NULL) { */
    /* builder->result[builder->result_size++] = (char) character; */
  /* } else { */
    /* rb_enc_mbcput(character, &builder->result[builder->result_size], encoding); */
    /* builder->result_size += size; */
  /* } */
/* } */

output_builder_t * make_builder() {
  return (output_builder_t *) malloc(sizeof(output_builder_t));
}

char * make_result_buffer(long len) {
  return (char *) malloc(len * sizeof(unsigned int));
}

static VALUE
str_camelize(VALUE self, VALUE rb_string, VALUE rb_uppercase_first_letter, VALUE rb_acronyms_array_length, VALUE rb_acronyms_array) {

  Check_Type(rb_string, T_STRING);

  //current_character_t *current_character = (current_character_t *) malloc(sizeof(current_character_t));
  output_builder_t *builder = make_builder();
  builder->result = make_result_buffer(RSTRING_LEN(rb_string));
  builder->result_size = 0;

  rb_encoding *encoding = rb_enc_from_index(ENCODING_GET(rb_string));
  char *string = RSTRING_PTR(rb_string);
  char *end = RSTRING_END(rb_string);
  int current_character_size;

  const int underscore = 95;
  char capitalize = rb_uppercase_first_letter == Qtrue ? 1 : 0;

  while (string < end) {
    unsigned int current_character = rb_enc_codepoint_len(string, end, &current_character_size, encoding);

    if (capitalize) {
      current_character = rb_enc_toupper(current_character, encoding);
      capitalize = 0;
    }

    if (current_character == underscore) {
      // skip and capitalize next character!
      capitalize = 1;
    } else {
      rb_enc_mbcput(current_character, &builder->result[builder->result_size], encoding);
      builder->result_size += current_character_size;
    }
    string += current_character_size;
  }
  //builder_flush(builder);

  VALUE result = rb_enc_str_new(builder->result, builder->result_size, encoding);

  //free(current_character);
  free(builder->result);
  free(builder);

  return result;
}

void
Init_fast_camelize(void) {
  VALUE rb_cFastCamelize = rb_define_module("FastCamelize");
  rb_define_singleton_method(rb_cFastCamelize, "camelize", str_camelize, 4);
}
