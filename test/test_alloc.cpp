
#include <frame.h>
#include <alloc.h>

#include <options.h>

#include <gtest/gtest.h>

TEST (alloc, string)
{
   ec_alloc_init(); // setup

   char * s = ec_alloc_string (3, "hello");
   ASSERT_STREQ ("hel", s);
}

TEST (alloc, string_max)
{
   int i;
   char * s;

   ec_alloc_init(); // setup

   for (i = 0; i < MAX_STRINGS; i++)
   {
      s = ec_alloc_string (i, "");
      ASSERT_TRUE (s != NULL);
   }

   s = ec_alloc_string (1, "");
   ASSERT_EQ (NULL, s);
}
