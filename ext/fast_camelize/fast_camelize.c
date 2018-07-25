#include <ruby.h>
#include <ruby/encoding.h>
#include <stdlib.h>
#include <string.h>

#define UNDERSCORE 95
#define DEBUG 1

typedef struct output_builder {
  enum state {
    START,
    LAST_CHARACTER_UNDERSCORE,
    LAST_CHARACTER_TEXT
  } current_state;

  char *result;
  long result_size;

  char *segment;
  long segment_size;

} output_builder_t;

output_builder_t * make_builder() {
  return (output_builder_t *) malloc(sizeof(output_builder_t));
}

char * make_result_buffer(long len) {
  return (char *) malloc(len * sizeof(unsigned int));
}

// http://layerlab.org/2018/01/30/how-i-tokenize-a-string-in-c.html
// str is a pointer to the beginning of your string. More specifically, it is a pointer to the position first character in some string that you want to parse.
// str_len is the length of that string. This does not have to be the full string, just the bit you care about.
// s the index of the start of your token
// e the index of the end of your token
// delim is the character that you want to delmit your string
int scan_s(char *str, long str_len, int *s, int *e, const char delim) {
  if (*e == str_len)
      return -1;
  for (*e = *s; *e <= str_len; *e+=1) {
      if ((str[*e] == delim) || (*e == str_len)) {
          return *e - *s;
      }
  }
  return -2;
}

static VALUE
str_camelize(VALUE self, VALUE rb_input, VALUE rb_uppercase_first_letter, VALUE rb_acronyms_array) {

  VALUE rb_string;

  switch (TYPE(rb_input)) {
    case T_STRING:
      rb_string = rb_input;
      break;
    case T_SYMBOL:
      rb_string = rb_sym_to_s(rb_input);
      break;
    default:
      rb_raise(rb_eRuntimeError, "wrong argument type");
  }

#ifdef DEBUG
  long int acronym_array_size = RARRAY_LEN(rb_acronyms_array);
  if (acronym_array_size) {
    printf("Got an array of size %ld\n", acronym_array_size);
    for (int i = 0; i < acronym_array_size; i++) {
      printf("index %d: %s\n", i, RSTRING_PTR(rb_ary_entry(rb_acronyms_array, i)));
    }
  }

  char *tok;
  int s = 0, e = 0;
  char *tmp_string = RSTRING_PTR(rb_string);
  while (scan_s(tmp_string, RSTRING_LEN(rb_string), &s, &e, '_') >= 0 ) {
    tok = (char *) malloc((e-s + 1) * sizeof(char));
    stpncpy(tok, tmp_string + s, e-s);
    tok[e-s]='\0';
    printf("%s\n", tok);
    free(tok);
    s = e + 1;
  }
#endif

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

  int first_character = 1;

  while (string < end) {
    unsigned int current_character = rb_enc_codepoint_len(string, end, &current_character_size, encoding);

    if (first_character) {
      if (rb_uppercase_first_letter) {
        current_character = rb_enc_toupper(current_character, encoding);
      } else {
        current_character = rb_enc_tolower(current_character, encoding);
      }
      capitalize = 0;
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
