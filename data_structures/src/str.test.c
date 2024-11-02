#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/list_extended.h"
#include "../include/memory.h"
#include "../include/str.h"

void test_str_splice() {
  printf("str splice\n");

  str_t s = "Hello World!";
  size_t start;
  size_t end;
  str_t actual;
  str_t expected;

  start = 3;
  end = 8;
  expected = "lo Wo";
  actual = str_splice(s, start, end);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  start = 5;
  end = 5;
  expected = "";
  actual = str_splice(s, start, end);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  start = 0;
  end = 0;
  expected = "";
  actual = str_splice(s, start, end);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  start = 12;
  end = 12;
  expected = "";
  actual = str_splice(s, start, end);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  start = 0;
  end = 12;
  expected = "Hello World!";
  actual = str_splice(s, start, end);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
}

void test_str_join() {
  printf("str join\n");

  size_t num_tokens = 10;
  str_t tokens[] = {
    str_wrap("I"),
    str_wrap("am"),
    str_wrap("a"),
    str_wrap("very"),
    str_wrap("happy"),
    str_wrap("fox"),
    str_wrap("that"),
    str_wrap("eats"),
    str_wrap("small"),
    str_wrap("animals."),
  };
  list_t *L = list_create(num_tokens);
  for (int i = 0; i < num_tokens; i++) {
    list_set(L, i, tokens[i]);
  }

  str_t s = str_join(L, " ");
  str_t expected = "I am a very happy fox that eats small animals.";
  assert(strcmp(s, expected) == 0);
  memory_free(s);

  for (int i = 0; i < num_tokens; i++) {
    memory_free(tokens[i]);
  }
  list_destroy(L);
}

void test_str_split() {
  printf("str split\n");

  list_t *L;
  str_t s;
  str_t delimiter;
  str_t actual;
  str_t expected;

  s = "apples >> bananas >> oranges >> grapes";
  delimiter = " >> ";
  expected = "['apples','bananas','oranges','grapes']";
  L = str_split(s, delimiter);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);

  s = "apples|bananas|oranges|grapes";
  delimiter = " >> ";
  expected = "['apples|bananas|oranges|grapes']";
  L = str_split(s, delimiter);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);

  s = "apples|bananas|oranges|grapes";
  delimiter = "";
  expected = "['apples|bananas|oranges|grapes']";
  L = str_split(s, delimiter);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);

  s = "";
  delimiter = " ";
  expected = "[]";
  L = str_split(s, delimiter);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);

  s = "";
  delimiter = "";
  expected = "[]";
  L = str_split(s, delimiter);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);
}

void test_str_splitlines() {
  printf("str splitlines\n");

  str_t s;
  str_t actual;
  str_t expected;
  list_t *L;

  s = "Sometimes I have\n seen monkeys\n cross the bridge.";
  expected = "['Sometimes I have',' seen monkeys',' cross the bridge.']";
  L = str_splitlines(s);
  actual = list_string(L, str_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, str_destroy);
}

void test_str_replace() {
  printf("str replace\n");

  str_t s;
  str_t sub1;
  str_t sub2;
  str_t actual;
  str_t expected;

  s = "apples >> bananas >> oranges >> grapes";
  sub1 = " >> ";
  sub2 = "|";
  expected = "apples|bananas|oranges|grapes"; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "apples|bananas|oranges|grapes";
  sub1 = " >> ";
  sub2 = "|";
  expected = "apples|bananas|oranges|grapes"; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "apples >> bananas >> oranges >> grapes";
  sub1 = "";
  sub2 = "|";
  expected = "apples >> bananas >> oranges >> grapes"; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  
  s = "apples >> bananas >> oranges >> grapes";
  sub1 = " >> ";
  sub2 = "";
  expected = "applesbananasorangesgrapes"; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "";
  sub1 = " ";
  sub2 = "|";
  expected = ""; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "";
  sub1 = "";
  sub2 = "|";
  expected = ""; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "";
  sub1 = "|";
  sub2 = "";
  expected = ""; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "";
  sub1 = "";
  sub2 = "";
  expected = ""; 
  actual = str_replace(s, sub1, sub2);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
}

void test_str_strip() {
  printf("str strip\n");

  str_t s;
  str_t actual;
  str_t expected;

  s = "\n \n \tHello my good friend.\n\t  \t\n";
  expected = "Hello my good friend.";
  actual = str_strip(s);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "Hello my good friend.";
  expected = "Hello my good friend.";
  actual = str_strip(s);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "\n \n \n";
  expected = "";
  actual = str_strip(s);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  s = "";
  expected = "";
  actual = str_strip(s);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
}

int main() {
  memory_pointers_init();

  test_str_splice();
  test_str_join();
  test_str_split();
  test_str_splitlines();
  test_str_replace();
  test_str_strip();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}
