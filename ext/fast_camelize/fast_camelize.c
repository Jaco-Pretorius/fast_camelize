#include <ruby.h>
#include <ruby/encoding.h>
#include <stdlib.h>

static VALUE
str_camelize(VALUE rb_string) {
  return rb_string;
}

static VALUE
rb_str_camelize(VALUE self, VALUE rb_string) {
  return str_camelize(rb_string);
}

void
Init_fast_camelize(void) {
  VALUE rb_cFastCamelize = rb_define_module("FastCamelize");
  rb_define_singleton_method(rb_cFastCamelize, "camelize", rb_str_camelize, 1);
  rb_define_method(rb_cString, "camelize", str_camelize, 0);
}
