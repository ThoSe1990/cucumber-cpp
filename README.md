# Coding With Thomas: Cucumber

This is a Cucumber interpreter written in C with C++ (17 or higher) bindings for ease of use. So my main goal was to have the basic implementation in C and an easy to use API for C++. More detailed documentation for the C++ API and how to use it in C will follow (I'm currently working on it).

## Disclaimer
This is a fun/educational project for me. After reading "Crafting Interpreters" by Robert Nystorm, I wanted a meaningful, not too complex example. Since I really like and use Cucumber, this seemed like a good project.
  

## CWT Cucumber for C++
The C++ API compiles its own `main`, which means you only need to implement the step definitions. There is also a `cucumber-no-main` target if you need your own main. I use a simple `box` class for demsontration (the `box` related methods should be self-explanatory). 

### Implementing Steps - First Example

Include the `cwt/cucumber.hpp` header and give each step a function name and a step name. The details (assertions, scenario context, defines) can be found [in the docs](...) or briefly below.

```cpp 
#include "cwt/cucumber.hpp"
#include "box.hpp"

GIVEN(box_init, "A box with {int} x {int} x {int}")
{
  const unsigned int w = CUKE_ARG(1);
  const unsigned int l = CUKE_ARG(2);
  const unsigned int h = CUKE_ARG(3);

  cuke::context<box>(w,l,h);
}

WHEN(box_open, "I open the box")
{
  cuke::context<box>().open(); 
}

THEN(box_is_open, "The box is open")
{
  cuke::is_true(cuke::context<box>().is_open());
}
```

And with this first feature file:

```gherkin
# ./examples/features/first_examples.feature

Feature: My First Feature File
  Just for demonstration

  Scenario: An arbitrary box
    Given A box with 2 x 2 x 2
    When I open the box
    Then The box is open 
```

Build the examples with CMake: 
```
cmake -S . -B ./build 
cmake --build ./build
```

We are ready to run this example:

```
$ ./build/bin/box ./examples/features/first_example.feature 

  Feature: My First Feature File  ./examples/features/first_example.feature:3

  Scenario: An arbitrary box  ./examples/features/first_example.feature:6
[   PASSED    ] A box with 2 x 2 x 2  ./examples/features/first_example.feature:7
[   PASSED    ] I open the box  ./examples/features/first_example.feature:8
[   PASSED    ] The box is open   ./examples/features/first_example.feature:9


1 Scenarios (1 passed)
3 Steps (3 passed)
```

### Steps / Value Access

To implement a step, use a define with a function name and a step name. There is no technical difference between these definitions:
- `GIVEN(function_name, "your step goes here")` 
- `WHEN(function_name, "your step goes here")` 
- `THEN(function_name, "your step goes here")` 
- Alternatively just use `STEP(function_name, "your step goes here")` 

Use `CUKE_ARG(..)` to access a value in a step. Start with index `1` for the first value. Use the corresponding type when you access the value, for instance:

```cpp
STEP(some_step, "A {string}, an {int} and a {float}")
{
  std::string str = CUKE_ARG(1);
  int i = CUKE_ARG(2);
  float f = CUKE_ARG(3);
  // ...
}
```

The values are implemted as [Cucumber expressions](https://github.com/cucumber/cucumber-expressions) and currently supported are: `{byte}`, `{short}`, `{int}`, `{long}`, `{float}`, `{double}`, `{string}`. 

Note: Use unsigned or const values in your implementation, if you need them:
```cpp
const unsigned int i = CUKE_ARG(2);
```

### Scenario Context

The scenario context or `cuke::context` stores an object for the duration of a scenario. After the scenario is finished, the object is destroyed. 

Recall these steps from the first example:

```cpp
GIVEN(box_init, "A box with {int} x {int} x {int}")
{
  const unsigned int w = CUKE_ARG(1);
  const unsigned int l = CUKE_ARG(2);
  const unsigned int h = CUKE_ARG(3);

  cuke::context<box>(w,l,h);
}
WHEN(box_open, "I open the box")
{
  cuke::context<box>().open(); 
}
```

We create in the given-step the `box` and forward all arguments to the constructor and in the when-step we modify the object to call the `open()` method from box. After the scenario the box is destroyed. 
  
This means: 

```cpp 
// this returns a reference to box
// if no box is in the context, the default constructor is called
cuke::context<box>();

// when you pass in arguments, and no box is in the context
// all values are forwarded to its constructor. 
// if a box already exists, the arguments are ignored.
cuke::context<box>(w,l,h);
```

In `cuke::context` you can put from each type one instance.

### Tags 

You can add tags to your Scenarios (and Scenario Outlines) as following:

```gherkin
# ./examples/features/tags.feature

Feature: Tags

  @small_boxes
  Scenario: An arbitrary box
    Given A box with 1 x 1 x 1
    Then The volume is 1

  @mid_sized_boxes
  Scenario: An arbitrary box
    Given A box with 10 x 15 x 12
    Then The volume is 1800

  @big_boxes
  Scenario: An arbitrary box
    Given A box with 90 x 80 x 70
    Then The volume is 504000
```

Now you can control the execution with `-t` or `--tags` with a bool condition inside the quotes and the corresponding tags:

```
$ ./build/bin/box ./examples/features/tags.feature -t "@small_boxes or @big_boxes"
```

The rules / syntax keywords are: 
- Write the tags with `@` symbol
- Logical operators: `and`, `or`, `xor`, `not`
- Parentheses `(`, `)`

So for instance this would be a valid statement: `"(@small_boxes and @big_boxes) or @mid_sized_boxes"`

If you don't pass `-t` or `--tags` to the program options, all Scenarios are executed.

### Hooks 

### Background

## Whats Missing

So what is missing? By now I can think of is:
- Comprehensive documentation (I'm working on that)
- `-h` / `--help` option 
- Languages (currently only english keywords are implemented)
- Rules
- Reports (json, ...) 
- Refactore, cleanups ... 
- ...


## And Finally ... 
... I'm currently on a sabbatical timeout from work and travel through Central America. If you open issues, find bugs or have anything for this project, please be patient. I have limited resources and time during traveling and in March/April 2024 I'm back home in my usual work environment.