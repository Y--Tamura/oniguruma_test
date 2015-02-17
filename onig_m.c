#include <stdio.h>
#include <string.h>
#include <oniguruma.h>
#include <sys/time.h>

double onig(UChar* pattern, UChar* str){
  int i, r;
  unsigned char *start, *range, *end;
  regex_t* reg;
  OnigErrorInfo einfo;
  OnigRegion *region;
  struct timeval start_timeval, end_timeval;
  double sec_all = 0;

  gettimeofday(&start_timeval, NULL);
  r = onig_new(&reg, pattern, pattern + strlen((char* )pattern),
  ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, &einfo);
#if 0
  if (r != ONIG_NORMAL) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(s, r, &einfo);
    fprintf(stderr, "ERROR: %s\n", s);
    return -1;
  }
#endif

  region = onig_region_new();

  end   = str + strlen((char* )str);
  start = str;
  range = end;
  r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);
  if (r >= 0) {
    printf("P, ");
  }
  else if (r == ONIG_MISMATCH) {
    printf("F, ");
  }
  else { /* error */
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(s, r);
    fprintf(stderr, "ERROR: %s\n", s);
  }
  gettimeofday(&end_timeval, NULL);
  sec_all = (end_timeval.tv_sec - start_timeval.tv_sec) + ((end_timeval.tv_usec - start_timeval.tv_usec) / 1000000.0);

  onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
  onig_free(reg);
  onig_end();
  return sec_all;
}

int main(int argc, char* argv[]){
  double times[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;

  for(i = 0; i < 5; i++){
    times[0] += onig((UChar* )"^(?:(?:(?:a)*)*)*c", (UChar* )"aaaaaaaaaaaac");
    times[1] += onig((UChar* )"^a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[2] += onig((UChar* )"a+a+a+a+a+a+a+a+a+a+a+a+a+a+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[3] += onig((UChar* )"(?:a+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaac");
    times[4] += onig((UChar* )"(?:(?:a)+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaac");
    times[5] += onig((UChar* )"(?:(?:aa)+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[6] += onig((UChar* )"((a)+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaac");
    times[7] += onig((UChar* )"(?:(?:(?:aa)+)+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[8] += onig((UChar* )"(((aa)+)+)+c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[9] += onig((UChar* )"(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)(?:a|a)c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[10] += onig((UChar* )"(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)(?:aa|a)c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac");
    times[11] += onig((UChar* )"(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)(aa|a)c", (UChar* )"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac");
    printf("\n");
  }

  for(i = 0; i < 12; i++){
    printf("%f[ms]\n", times[i]*1000/5);
  }
  return 0;
}
