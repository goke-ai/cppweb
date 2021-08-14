#pragma once

#include <stdio.h>
#include <malloc.h>
#include <vector>
#include "../entities/contact.h"

using namespace entities;

std::vector<Contact> gDB = {Contact(1, "AA Goke", "+234803"),
                            Contact(2, "BB Lillian", "+2348103"),
                            Contact(3, "CC Lolade", "+2348104"),
                            Contact(4, "DD Gbola", "+2348024")};


// char * format(const char *fmt, ...)
// {
//     int count;
//    int size;
//    char *s = NULL;

//     sprintf()
//    count = _scprintf( fmt, M_PI);

//    size = count + 1; // the string will need one more char for the null terminator
//    s = malloc(sizeof(char) * size);
//    sprintf_s(s, size, "The value of Pi is calculated to be %f.\n",
//                       M_PI);
//    printf("The length of the following string will be %i.\n", count);
//    printf("%s", s);
//    free( s );
// }