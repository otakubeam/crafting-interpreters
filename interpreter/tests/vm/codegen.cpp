#include <types/check/type_checker.hpp>

#include <vm/codegen/compiler.hpp>
#include <vm/interpreter.hpp>

#include <parse/parser.hpp>
#include <lex/lexer.hpp>

// Finally,
#include <catch2/catch.hpp>

extern bool print_debug_info;

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen: push constant", "[vm]") {
  char stream[] = "1";
  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto pr = p.ParsePrimary();

  vm::ExecutableChunk chunk{
      .instructions =
          {
              vm::Instr{.type = vm::InstrType::PUSH_STACK,  //
                        .arg1 = 0}                          //
          },                                                //
      //
      .attached_vals{
          vm::rt::PrimitiveValue{.tag = vm::rt::ValueTag::Int,  //
                                 .as_int = 1},                  //
      },
  };

  vm::codegen::Compiler c;
  auto res = c.Compile(pr);

  CHECK(chunk.instructions[0].type == res.instructions[0].type);
  CHECK(chunk.attached_vals[0].tag == res.attached_vals[0].tag);
  CHECK(chunk.attached_vals[0].as_int == res.attached_vals[0].as_int);
  CHECK(vm::BytecodeInterpreter::InterpretStandalone({res}) == 1);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:add", "[vm]") {
  char stream[] = "1 + 2";
  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto pr = p.ParseExpression();

  vm::codegen::Compiler c;
  auto res = c.Compile(pr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone({res}) == 3);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:if", "[vm:codegen]") {
  char stream[] = "if true { 2 }";
  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto pr = p.ParseExpression();

  vm::codegen::Compiler c;
  auto res = c.Compile(pr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone({res}) == 2);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:if-else-true", "[vm:codegen]") {
  char stream[] = "if true { 2 } else { 3 }";
  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto pr = p.ParseExpression();

  vm::codegen::Compiler c;
  auto res = c.Compile(pr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone({res}) == 2);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:if-else-false", "[vm:codegen]") {
  char stream[] = "if false { 2 } else { 3 }";
  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto pr = p.ParseExpression();

  vm::codegen::Compiler c;
  auto res = c.Compile(pr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone({res}) == 3);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:function", "[vm:codegen]") {
  // char stream[] =
  //     "{                                                "
  //     "  fun f(i: Int) Int {                            "
  //     "      i + i                                      "
  //     "  }                                              "
  //     "                                                 "
  //     "  f(23)                                          "
  //     "}                                                ";
  // std::stringstream source{stream};
  // Parser p{lex::Lexer{source}};
  //
  // auto expr = p.ParseExpression();
  //
  // types::check::TypeChecker tchk;
  // CHECK_NOTHROW(tchk.Eval(expr));
  //
  // vm::codegen::Compiler c;
  // auto res = c.CompileScript(expr);
  //
  // // for (auto r : *res) {
  // //   r.Print();
  // // }
  //
  // CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 46);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:local", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  fun f(b: Bool) Int {                           "
      "      var res = if b { 100 } else { 101 };       "
      "      res + res                                  "
      "  }                                              "
      "                                                 "
      "  f(true)                                        "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 200);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:assignment", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  var x = 5;                                     "
      "  x = 6;                                         "
      "  x                                              "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 6);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:struct", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  struct Str {                                   "
      "     count: Int,                                 "
      "     ismodified: Bool,                           "
      "  };                                             "
      "                                                 "
      "  var instance = Str:{4, false};                 "
      "  instance.ismodified = true;                    "
      "  instance.ismodified                            "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 1);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:struct:nested", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  struct Inner {                                 "
      "     fieldOne: Int,                              "
      "     fieldTwo: Int,                              "
      "  };                                             "
      "                                                 "
      "  struct Str {                                   "
      "     count: Int,                                 "
      "     inner: Inner,                               "
      "     ismodified: Bool,                           "
      "  };                                             "
      "                                                 "
      "  var instance = Str:{4, Inner:{0,1}, false};    "
      "  instance.inner.fieldTwo = 123;                 "
      "  instance.ismodified  = true;                   "
      "  instance.ismodified                            "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 1);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:struct:copy", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  struct Str {                                   "
      "     ismodified: Bool,                           "
      "     count: Int,                                 "
      "  };                                             "
      "                                                 "
      "  var instance = Str:{true, 12};                 "
      "  var instance2 = instance;                      "
      "                                                 "
      "  instance.count = instance.count + 1;           "
      "  instance2.count                                "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 12);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:struct:fn-arg", "[vm:codegen]") {
  char stream[] =
      "{                                                "
      "  struct Str {                                   "
      "     ismodified: Bool,                           "
      "     count: Int,                                 "
      "  };                                             "
      "                                                 "
      "  fun f(s: Str, ss: Str) Int {                   "
      "     s.ismodified = false;                       "
      "     ss.ismodified = false;                      "
      "     ss.count = 123;                             "
      "     s.count + ss.count                          "
      "  }                                              "
      "                                                 "
      "  var inst = Str:{true, 12};                     "
      "                                                 "
      "  f(inst, inst)                                  "
      "}                                                ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 135);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:struct:fn-arg:nested", "[vm:codegen]") {
  char stream[] =
      " {                                         "
      "     struct Str {                          "
      "        guard: Int,                        "
      "        count: Int,                        "
      "     };                                    "
      "                                           "
      "     fun g(i: Str) Int {                   "
      "        (i.count + 1)                      "
      "     }                                     "
      "                                           "
      "     fun f(i: Str, g: (Str) Int) Int {     "
      "        g(i)                               "
      "     }                                     "
      "                                           "
      "     var inst = Str:{1234, 3};             "
      "                                           "
      "     f(inst, g)                            "
      " }                                         ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 4);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:print", "[vm:codegen]") {
  char stream[] =
      " {                                         "
      "     print(123);                           "
      " }                                         ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK_NOTHROW(vm::BytecodeInterpreter::InterpretStandalone(*res));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:ptr", "[vm:codegen]") {
  char stream[] =
      " {                                         "
      "     var a = 5;                            "
      "     var ptrA = &a;                        "
      "     *ptrA = 4;                            "
      "     a                                     "
      " }                                         ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 4);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:ptr:init", "[vm:codegen]") {
  char stream[] =
      " {                                               "
      "   struct Tree {                                 "
      "     left: *Tree,                                "
      "     right: *Tree,                               "
      "   };                                            "
      "                                                 "
      "   var tree = Tree:{unit, unit};                 "
      " }                                               ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK_NOTHROW(vm::BytecodeInterpreter::InterpretStandalone(*res));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:recursive", "[vm:codegen]") {
  char stream[] =
      " {                                             "
      "    fun mul(a: Int, b: Int) Int {              "
      "       if a == 1 {                             "
      "          b                                    "
      "       } else {                                "
      "          b + mul(a-1, b)                      "
      "       }                                       "
      "    }                                          "
      "                                               "
      "    mul(3, 5)                                "
      " }                                             ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 15);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("vm:codegen:binsearch:function", "[vm:codegen]") {
  char stream[] =
      "{                                                  "
      "   struct Tree {                                   "
      "      left: *Tree,                                 "
      "      right: *Tree,                                "
      "      value: Int,                                  "
      "   };                                              "
      "                                                   "
      "   fun binSearchInt(tree: *Tree, val: Int) Bool {  "
      "       if isNull(tree) {                           "
      "           return false;                           "
      "       };                                          "
      "                                                   "
      "       var nodeValue = (*tree).value;              "
      "                                                   "
      "       if nodeValue == val {                       "
      "          return true;                             "
      "       };                                          "
      "                                                   "
      "       var subtree = if nodeValue < val {          "
      "           (*tree).left                            "
      "       } else {                                    "
      "           (*tree).right                           "
      "       };                                          "
      "                                                   "
      "       binSearchInt(subtree, val)                  "
      "   }                                               "
      "                                                   "
      "   0                                               "
      "}                                                  ";

  std::stringstream source{stream};
  Parser p{lex::Lexer{source}};

  auto expr = p.ParseExpression();

  types::check::TypeChecker tchk;
  CHECK_NOTHROW(tchk.Eval(expr));

  vm::codegen::Compiler c;
  auto res = c.CompileScript(expr);

  CHECK(vm::BytecodeInterpreter::InterpretStandalone(*res) == 00);
}

//////////////////////////////////////////////////////////////////////
