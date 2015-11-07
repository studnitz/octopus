# General conventions

## Indentation
Code should be indented with 4 Spaces.

We try not to indent many levels deep to keep legibility, instead we try to make those structures easier to read/less needy to indent.

## Line width
A line shouldn't exceed 80 characters.

## Header files

We always use header files for our classes.

## Braces

if/else-, for-, while- and case-statements should look like this:

    if (something) {
        doSomething();
        doAnotherThing();
    } else {
        doElseThing();
    }
    
    for (int i=0; i < maxValue; i++) {
        doSomethingInFor();
    }

# Naming conventions

## Files

Files should be named like this: `myClass.cpp` and `myClass.h`

## Types
Types should be MixedCase with a upper case start.

    DatabaseObject, Line // good

## Methods
Methods should be mixedCase with a lower case start and verbs.

    hasSomething() // good
    Hello() //bad

## Variables
Variables should be mixedCase with a lower case start.

    line, counter, lastName // good

Variables that refer to a set of objects should be in plural form.

    Vector<Type> points // good
    Vector<Type> point // bad

## Namespaces
namespaces should be all lowercase.

    de::bp::videocapture
## General
- Boolean variables should always be in positive form: `isTrue`, not `isNotTrue`
- Iterator variables should be called `i, j, k` etc.
- Boolean variables and functions should have the prefix `is` (or `has, can, should`)
