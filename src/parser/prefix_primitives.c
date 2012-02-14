#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "prefix_primitives.h"

int prefix_is_exactly(char *src, char* pre) {
  int p;
  for (p = 0; pre[p] && src[p] == pre[p]; p++) ;
  return pre[p] ? 0 : p;
}

int prefix_is_one_of(char *src, char *class) {
  int i;
  for (i = 0; class[i] && src[0] != class[i]; i++) ;
  return class[i] ? 1 : 0;
}

int prefix_is_some_of(char *src, char *class) {
  int p;
  for (p = 0; prefix_is_one_of(src+p, class); p++) ;
  return p;
}

int prefix_is_delimited_by(char *src, char *beg, char *end, int esc) {
  int p, len  = prefix_is_exactly(src, beg);
  if (!len) return 0;
  p = len;
  while (1) {
    if (src[p] == '\0') return 0;
    len = prefix_is_exactly(src+p, end);
    if (len && (!esc || src[p-1] != '\\')) return p + len;
    p += len ? len : 1;
  }
}

int prefix_try_alternatives(char *src, ...) {
  int p = 0;
  va_list ap;
  va_start(ap, src);
  prefix_matcher m = va_arg(ap, prefix_matcher);  
  while (m && !(p = (*m)(src))) m = va_arg(ap, prefix_matcher);
  return p;
}

DEFINE_SINGLE_CTYPE_MATCHER(space);
DEFINE_SINGLE_CTYPE_MATCHER(alpha);
DEFINE_SINGLE_CTYPE_MATCHER(digit);
DEFINE_SINGLE_CTYPE_MATCHER(xdigit);
DEFINE_SINGLE_CTYPE_MATCHER(alnum);
DEFINE_SINGLE_CTYPE_MATCHER(punct);
DEFINE_CTYPE_SEQUENCE_MATCHER(space);
DEFINE_CTYPE_SEQUENCE_MATCHER(alpha);
DEFINE_CTYPE_SEQUENCE_MATCHER(digit);
DEFINE_CTYPE_SEQUENCE_MATCHER(xdigit);
DEFINE_CTYPE_SEQUENCE_MATCHER(alnum);
DEFINE_CTYPE_SEQUENCE_MATCHER(punct);
DEFINE_TO_EOL_MATCHER(line_comment, "//");
DEFINE_DELIMITED_MATCHER(block_comment, "/*", "*/", 0);
DEFINE_DELIMITED_MATCHER(double_quoted_string, "\"", "\"", 1);
DEFINE_DELIMITED_MATCHER(single_quoted_string, "'", "'", 1);
DEFINE_DELIMITED_MATCHER(interpolant, "#{", "}", 0);

int prefix_is_string(char *src) {
  return prefix_try_alternatives(src,
                                 prefix_is_double_quoted_string,
                                 prefix_is_single_quoted_string);
}

DEFINE_EXACT_MATCHER(lparen, "(");
DEFINE_EXACT_MATCHER(rparen, ")");
DEFINE_EXACT_MATCHER(lbrack, "[");
DEFINE_EXACT_MATCHER(rbrack, "]");
DEFINE_EXACT_MATCHER(lbrace, "{");
DEFINE_EXACT_MATCHER(rbrace, "}");