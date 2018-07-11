#include <ruby.h>
#include <ruby/encoding.h>
#include <stdlib.h>

#define UNDERSCORE 95

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
    /* if (builder.pushNext == 1) { */
      /* builder.pushNext = 0; */
      /* builder_result_push_literal(builder, '_'); */
    /* } */

    /* builder.result[builder.result_size++] = (char) character - 'A' + 'a'; */
    /* return; */
  /* } */

  /* builder.pushNext = (character_is_lower(character) || character_is_digit(character)); */

  /* if (encoding == NULL) { */
    /* builder.result[builder.result_size++] = (char) character; */
  /* } else { */
    /* rb_enc_mbcput(character, &builder.result[builder.result_size], encoding); */
    /* builder.result_size += size; */
  /* } */
/* } */

output_builder_t * make_builder() {
  return (output_builder_t *) malloc(sizeof(output_builder_t));
}

char * make_result_buffer(long len) {
  return (char *) malloc(len * sizeof(unsigned int));
}

static VALUE
str_camelize(VALUE self, VALUE rb_input, VALUE rb_uppercase_first_letter, VALUE rb_acronyms_array) {

#ifdef DEBUG
  long int acronym_array_size = RARRAY_LEN(rb_acronyms_array);
  if (acronym_array_size) {
    printf("Got an array of size %ld\n", acronym_array_size);
    for (int i = 0; i < acronym_array_size; i++) {
      printf("index %d: %s\n", i, RSTRING_PTR(rb_ary_entry(rb_acronyms_array, i)));
    }
  }
#endif

  VALUE rb_string;

  switch (TYPE(rb_input)) {
    case T_STRING:
      rb_string = rb_input;
      break;
    case T_SYMBOL:
      rb_string = rb_sym_to_s(rb_input);
      break;
    default:
      rb_raise(rb_eRuntimeError, "wrong argument type ");
  }

  char resultBuffer[1024];

  output_builder_t builder;
  int need_to_free_builder_buffer = 0;
  if (RSTRING_LEN(rb_string) < 1024) {
    builder.result = resultBuffer;
    need_to_free_builder_buffer = 0;
  } else {
    builder.result = make_result_buffer(RSTRING_LEN(rb_string));
    need_to_free_builder_buffer = 1;
  }
  builder.result_size = 0;

  rb_encoding *encoding = rb_enc_from_index(ENCODING_GET(rb_string));
  char *string = RSTRING_PTR(rb_string);
  char *end = RSTRING_END(rb_string);
  int current_character_size;

  char capitalize = RTEST(rb_uppercase_first_letter);

  const int underscore = 95;
  char capitalize = 0;
  int first_character = 1;

  while (string < end) {
    unsigned int current_character = rb_enc_codepoint_len(string, end, &current_character_size, encoding);

    if (first_character) {
      if (rb_uppercase_first_letter) {
        current_character = rb_enc_toupper(current_character, encoding);
      } else {
        current_character = rb_enc_tolower(current_character, encoding);
      }
    } else if (capitalize) {
      current_character = rb_enc_toupper(current_character, encoding);
      capitalize = 0;
    } else {
      current_character = rb_enc_tolower(current_character, encoding);
    }

    if (current_character == UNDERSCORE) {
      // skip and capitalize next character!
      capitalize = 1;
    } else {
      rb_enc_mbcput(current_character, &builder.result[builder.result_size], encoding);
      builder.result_size += current_character_size;
    }
    string += current_character_size;

    first_character = 0;
  }

  VALUE result = rb_enc_str_new(builder.result, builder.result_size, encoding);

  if (need_to_free_builder_buffer) {
    free(builder.result);
  }

  return result;
}

void
Init_fast_camelize(void) {
  VALUE rb_cFastCamelize = rb_define_module("FastCamelize");
  rb_define_singleton_method(rb_cFastCamelize, "camelize", str_camelize, 3);
}
