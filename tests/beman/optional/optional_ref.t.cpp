// tests/beman/optional/optional_ref.t.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/optional/optional.hpp>
#include <beman/optional/test_types.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("OptionalRefTest: Constructors", "[optional]") {
    beman::optional::optional<int&> i1;
    beman::optional::optional<int&> i2{beman::optional::nullopt};
    std::ignore = i1;
    std::ignore = i2;

    int                             i  = 0;
    beman::optional::optional<int&> i3 = i;
    std::ignore                        = i3;

    using beman::optional::tests::empty;

    beman::optional::optional<empty&> e1;
    beman::optional::optional<empty&> e2{beman::optional::nullopt};
    std::ignore = e1;
    std::ignore = e2;

    empty                             e{};
    beman::optional::optional<empty&> e3 = e;
    std::ignore                          = e3;

    using beman::optional::tests::no_default_ctor;

    beman::optional::optional<no_default_ctor&> nd1;
    beman::optional::optional<no_default_ctor&> nd2{beman::optional::nullopt};
    std::ignore = nd1;
    std::ignore = nd2;

    no_default_ctor no_def{e};

    beman::optional::optional<no_default_ctor&> nd3 = no_def;
    std::ignore                                     = nd3;

    beman::optional::optional<int&> ie;
    beman::optional::optional<int&> i4 = ie;
    CHECK(!i4);

    using beman::optional::tests::base;
    using beman::optional::tests::derived;

    base                             b{1};
    derived                          d(1, 2);
    beman::optional::optional<base&> b1 = b;
    beman::optional::optional<base&> b2 = d;

    beman::optional::optional<derived&> d2 = d;
    beman::optional::optional<base&>    b3 = d2;
    beman::optional::optional<base&>    b4{d2};

    beman::optional::optional<derived&> empty;
    beman::optional::optional<base&>    fromEmpty(empty);
    beman::optional::optional<base&>    fromEmpty2 = empty;

    /*
     * template <class U>
     *   requires(!is_derived_from_optional<decay_t<U>>)
     * constexpr explicit(!is_convertible_v<U, T>) optional(U&& u) noexcept;
     *
     * Not selected -- use default constructor of optional<T&> so it is
     * *DIS*engaged
     */

    beman::optional::optional<int&> tempint = {};
    CHECK(!tempint);
}

namespace {
// https://gcc.godbolt.org/z/aGGW3TYov
struct derived;
extern derived d;
struct base {
    virtual ~base() = default;
    operator derived&() { return d; }
};

struct derived : base {};

derived d;
} // namespace

struct Thing {};
beman::optional::optional<Thing&> process() {
    static Thing t;
    return t;
}

beman::optional::optional<Thing&> processEmpty() { return beman::optional::nullopt; }

TEST_CASE("OptionalRefTest: BaseDerivedCastConstruction", "[optional]") {
    base                                b;
    derived&                            dref(b); // ok
    beman::optional::optional<derived&> dopt1(b);
    beman::optional::optional<derived&> dopt2(dref);
    CHECK(dopt1.has_value());
    CHECK(dopt2.has_value());
}

TEST_CASE("OptionalRefTest: BaseDerivedCastEmplace", "[optional]") {
    base                                b;
    derived&                            dref(b); // ok
    beman::optional::optional<derived&> dopt1;
    dopt1.emplace(b);
    beman::optional::optional<derived&> dopt2;
    dopt2.emplace(dref);
    CHECK(dopt1.has_value());
    CHECK(dopt2.has_value());
}

TEST_CASE("OptionalRefTest: Assignment", "[optional]") {
    beman::optional::optional<int&> i1;
    CHECK(!i1);
    int i = 5;
    i1    = i;
    i     = 7;
    CHECK(i1);
    CHECK((*i1 = 7));
    CHECK(7 == i1);

    double d;
    // i1 = d;  // ill-formed by mandate
    beman::optional::optional<double&> d1 = d;
    // i1 = d1; // ill-formed by mandate
    beman::optional::optional<int&> i2 = i1;
    CHECK(i2);
    CHECK((*i2 = 7));

    beman::optional::optional<int&> empty;
    CHECK(!empty);
    i2 = empty;
    CHECK(!i2);
    int  eight  = 8;
    int& result = empty.emplace(eight);
    CHECK(empty);
    CHECK(8 == empty);
    CHECK(&eight == &result);

    beman::optional::optional<const Thing&> o;
    CHECK(!o);
    o = process(); // well-formed
    CHECK(o);

    o = processEmpty(); // well-formed
    CHECK(!o);

    beman::optional::optional<const int&> o2;
    CHECK(!o2);
    o2 = [&]() { return i1; }();

    CHECK(7 == *o2);
}

TEST_CASE("OptionalRefTest: NullOptAssignment", "[optional]") {
    beman::optional::optional<int&> i1;
    CHECK(!i1);
    int i = 5;
    i1    = i;

    CHECK(i1);
    i1 = beman::optional::nullopt;
    CHECK(!i1);
    i1 = i;
    CHECK(i1);
}

TEST_CASE("OptionalRefTest: ConstRefAssignment", "[optional]") {
    int                                   i = 7;
    beman::optional::optional<int&>       i1{i};
    const beman::optional::optional<int&> i2 = i1;

    beman::optional::optional<const int&> c1;
    c1 = i2;
    CHECK(c1);
    CHECK(7 == *c1);

    i = 5;
    CHECK(5 == *c1);
    const beman::optional::optional<int&> empty(beman::optional::nullopt);
    c1 = empty;
    CHECK(!c1);
}

TEST_CASE("OptionalRefTest: ConvertingConstRvalRef", "[optional]") {
    int                                   i = 7;
    beman::optional::optional<int&>       i1{i};
    const beman::optional::optional<int&> i2 = i1;

    beman::optional::optional<const int&> c1;
    c1 = std::move(i2);
    CHECK(c1);
    CHECK(7 == *c1);

    i = 5;
    CHECK(5 == *c1);
    const beman::optional::optional<int&> empty(beman::optional::nullopt);
    c1 = std::move(empty);
    CHECK(!c1);
}

TEST_CASE("OptionalRefTest: NullOptConstruction", "[optional]") {
    beman::optional::optional<int&> i1(beman::optional::nullopt);
    CHECK(!i1);
    int i = 5;
    i1    = i;

    CHECK(i1);
    i1 = beman::optional::nullopt;
    CHECK(!i1);
    i1 = i;
    CHECK(i1);
}

TEST_CASE("OptionalRefTest: RelationalOps", "[optional]") {
    int                             i1 = 4;
    int                             i2 = 42;
    beman::optional::optional<int&> o1{i1};
    beman::optional::optional<int&> o2{i2};
    beman::optional::optional<int&> o3{};

    //  SECTION("self simple")
    {
        CHECK(!(o1 == o2));
        CHECK(o1 == o1);
        CHECK(o1 != o2);
        CHECK(!(o1 != o1));
        CHECK(o1 < o2);
        CHECK(!(o1 < o1));
        CHECK(!(o1 > o2));
        CHECK(!(o1 > o1));
        CHECK(o1 <= o2);
        CHECK(o1 <= o1);
        CHECK(!(o1 >= o2));
        CHECK(o1 >= o1);
    }
    //  SECTION("nullopt simple")
    {
        CHECK(!(o1 == beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt == o1));
        CHECK(o1 != beman::optional::nullopt);
        CHECK(beman::optional::nullopt != o1);
        CHECK(!(o1 < beman::optional::nullopt));
        CHECK(beman::optional::nullopt < o1);
        CHECK(o1 > beman::optional::nullopt);
        CHECK(!(beman::optional::nullopt > o1));
        CHECK(!(o1 <= beman::optional::nullopt));
        CHECK(beman::optional::nullopt <= o1);
        CHECK(o1 >= beman::optional::nullopt);
        CHECK(!(beman::optional::nullopt >= o1));

        CHECK(o3 == beman::optional::nullopt);
        CHECK(beman::optional::nullopt == o3);
        CHECK(!(o3 != beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt != o3));
        CHECK(!(o3 < beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt < o3));
        CHECK(!(o3 > beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt > o3));
        CHECK(o3 <= beman::optional::nullopt);
        CHECK(beman::optional::nullopt <= o3);
        CHECK(o3 >= beman::optional::nullopt);
        CHECK(beman::optional::nullopt >= o3);
    }
    //  SECTION("with T simple")
    {
        CHECK(!(o1 == 1));
        CHECK(!(1 == o1));
        CHECK(o1 != 1);
        CHECK(1 != o1);
        CHECK(!(o1 < 1));
        CHECK(1 < o1);
        CHECK(o1 > 1);
        CHECK(!(1 > o1));
        CHECK(!(o1 <= 1));
        CHECK(1 <= o1);
        CHECK(o1 >= 1);
        CHECK(!(1 >= o1));

        CHECK(o1 == 4);
        CHECK(4 == o1);
        CHECK(!(o1 != 4));
        CHECK(!(4 != o1));
        CHECK(!(o1 < 4));
        CHECK(!(4 < o1));
        CHECK(!(o1 > 4));
        CHECK(!(4 > o1));
        CHECK(o1 <= 4);
        CHECK(4 <= o1);
        CHECK(o1 >= 4);
        CHECK(4 >= o1);
    }
    std::string                             s4 = "hello";
    std::string                             s5 = "xyz";
    beman::optional::optional<std::string&> o4{s4};
    beman::optional::optional<std::string&> o5{s5};

    //  SECTION("self complex")
    {
        CHECK(!(o4 == o5));
        CHECK(o4 == o4);
        CHECK(o4 != o5);
        CHECK(!(o4 != o4));
        CHECK(o4 < o5);
        CHECK(!(o4 < o4));
        CHECK(!(o4 > o5));
        CHECK(!(o4 > o4));
        CHECK(o4 <= o5);
        CHECK(o4 <= o4);
        CHECK(!(o4 >= o5));
        CHECK(o4 >= o4);
    }
    //  SECTION("nullopt complex")
    {
        CHECK(!(o4 == beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt == o4));
        CHECK(o4 != beman::optional::nullopt);
        CHECK(beman::optional::nullopt != o4);
        CHECK(!(o4 < beman::optional::nullopt));
        CHECK(beman::optional::nullopt < o4);
        CHECK(o4 > beman::optional::nullopt);
        CHECK(!(beman::optional::nullopt > o4));
        CHECK(!(o4 <= beman::optional::nullopt));
        CHECK(beman::optional::nullopt <= o4);
        CHECK(o4 >= beman::optional::nullopt);
        CHECK(!(beman::optional::nullopt >= o4));

        CHECK(o3 == beman::optional::nullopt);
        CHECK(beman::optional::nullopt == o3);
        CHECK(!(o3 != beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt != o3));
        CHECK(!(o3 < beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt < o3));
        CHECK(!(o3 > beman::optional::nullopt));
        CHECK(!(beman::optional::nullopt > o3));
        CHECK(o3 <= beman::optional::nullopt);
        CHECK(beman::optional::nullopt <= o3);
        CHECK(o3 >= beman::optional::nullopt);
        CHECK(beman::optional::nullopt >= o3);
    }

    //  SECTION("with T complex")
    {
        CHECK(!(o4 == "a"));
        CHECK(!("a" == o4));
        CHECK(o4 != "a");
        CHECK("a" != o4);
        CHECK(!(o4 < "a"));
        CHECK("a" < o4);
        CHECK(o4 > "a");
        CHECK(!("a" > o4));
        CHECK(!(o4 <= "a"));
        CHECK("a" <= o4);
        CHECK(o4 >= "a");
        CHECK(!("a" >= o4));

        CHECK(o4 == "hello");
        CHECK("hello" == o4);
        CHECK(!(o4 != "hello"));
        CHECK(!("hello" != o4));
        CHECK(!(o4 < "hello"));
        CHECK(!("hello" < o4));
        CHECK(!(o4 > "hello"));
        CHECK(!("hello" > o4));
        CHECK(o4 <= "hello");
        CHECK("hello" <= o4);
        CHECK(o4 >= "hello");
        CHECK("hello" >= o4);
    }
}

TEST_CASE("OptionalRefTest: Triviality", "[optional]") {
    CHECK(std::is_trivially_copy_constructible<beman::optional::optional<int&>>::value);
    CHECK(std::is_trivially_copy_assignable<beman::optional::optional<int&>>::value);
    CHECK(std::is_trivially_move_constructible<beman::optional::optional<int&>>::value);
    CHECK(std::is_trivially_move_assignable<beman::optional::optional<int&>>::value);
    CHECK(std::is_trivially_destructible<beman::optional::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        CHECK(std::is_trivially_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_copy_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_move_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_move_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_destructible<beman::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&) {}
            T(T&&) {};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T() {}
        };
        CHECK(std::is_trivially_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_copy_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_move_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_move_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_trivially_destructible<beman::optional::optional<T&>>::value);

        CHECK((std::is_trivially_constructible<beman::optional::optional<T&>, beman::optional::optional<T&>&>::value));
    }
}

TEST_CASE("OptionalRefTest: Deletion", "[optional]") {
    CHECK(std::is_copy_constructible<beman::optional::optional<int&>>::value);
    CHECK(std::is_copy_assignable<beman::optional::optional<int&>>::value);
    CHECK(std::is_move_constructible<beman::optional::optional<int&>>::value);
    CHECK(std::is_move_assignable<beman::optional::optional<int&>>::value);
    CHECK(std::is_destructible<beman::optional::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        CHECK(std::is_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_copy_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_destructible<beman::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        CHECK(std::is_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_copy_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_assignable<beman::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        CHECK(std::is_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_copy_assignable<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_move_assignable<beman::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        CHECK(std::is_copy_constructible<beman::optional::optional<T&>>::value);
        CHECK(std::is_copy_assignable<beman::optional::optional<T&>>::value);
    }
}

struct takes_init_and_variadic {
    std::vector<int>     v;
    std::tuple<int, int> t;
    template <class... Args>
    takes_init_and_variadic(std::initializer_list<int> l, Args&&... args) : v(l), t(std::forward<Args>(args)...) {}
};

TEST_CASE("OptionalRefTest: Nullopt", "[optional]") {
    beman::optional::optional<int&> o1 = beman::optional::nullopt;
    beman::optional::optional<int&> o2{beman::optional::nullopt};
    beman::optional::optional<int&> o3(beman::optional::nullopt);
    beman::optional::optional<int&> o4 = {beman::optional::nullopt};

    CHECK(!o1);
    CHECK(!o2);
    CHECK(!o3);
    CHECK(!o4);

    CHECK(!std::is_default_constructible<beman::optional::nullopt_t>::value);
}

TEST_CASE("OptionalRefTest: Observers", "[optional]") {
    int                                   var = 42;
    beman::optional::optional<int&>       o1  = var;
    beman::optional::optional<int&>       o2;
    const beman::optional::optional<int&> o3 = var;
    const beman::optional::optional<int&> o4;
    int                                   var2 = 42;
    int                                   var3 = 6 * 9;
    CHECK(*o1 == 42);
    CHECK(*o1 == o1.value());
    CHECK(o2.value_or(var2) == 42);
    CHECK(o3.value() == 42);
    CHECK(o3.value_or(var3) == 42);
    CHECK(o4.value_or(var3) == 54);
    int j = 99;
    CHECK(o4.value_or(j) == 99);
    // o4.value_or(j) = 88;
    // CHECK (j == 88);
    int var99 = 99;
    j         = 88;
    CHECK([&]() {
        beman::optional::optional<int&> o(j);
        return o;
    }()
              .value_or(var99) == 88);

    CHECK([&]() {
        beman::optional::optional<int&> o;
        return o;
    }()
              .value_or(var99) == 99);

    CHECK(o3.value_or([&]() -> int& { return var3; }()) == 42);
    CHECK(o4.value_or([&]() -> int& { return var3; }()) == 54);

    std::string                             meow{"meow"};
    std::string                             bark{"bark"};
    beman::optional::optional<std::string&> so1;
    beman::optional::optional<std::string&> so2{meow};
    auto                                    t1 = so1.value_or(bark);
    auto                                    t2 = so2.value_or(bark);
    // auto t3 = so1.value_or("bark");
    // auto t4 = so2.value_or("bark");
    // std::tuple<const std::string&> t("meow");

    auto t5 = so1.value_or({});
    auto t6 = so2.value_or({});
    CHECK(std::string() == t5);
    CHECK(meow == t6);

    auto success = std::is_same<decltype(o1.value()), int&>::value;
    static_assert(std::is_same<decltype(o1.value()), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(o2.value()), int&>::value;
    static_assert(std::is_same<decltype(o2.value()), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(o3.value()), int&>::value;
    static_assert(std::is_same<decltype(o3.value()), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(std::move(o1).value()), int&>::value;
    static_assert(std::is_same<decltype(std::move(o1).value()), int&>::value);
    CHECK(success);

    success = std::is_same<decltype(*o1), int&>::value;
    static_assert(std::is_same<decltype(*o1), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(*o2), int&>::value;
    static_assert(std::is_same<decltype(*o2), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(*o3), int&>::value;
    static_assert(std::is_same<decltype(*o3), int&>::value);
    CHECK(success);
    success = std::is_same<decltype(*std::move(o1)), int&>::value;
    static_assert(std::is_same<decltype(*std::move(o1)), int&>::value);
    CHECK(success);

    success = std::is_same<decltype(o1.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o1.operator->()), int*>::value);
    CHECK(success);
    success = std::is_same<decltype(o2.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o2.operator->()), int*>::value);
    CHECK(success);
    success = std::is_same<decltype(o3.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o3.operator->()), int*>::value);
    CHECK(success);
    success = std::is_same<decltype(std::move(o1).operator->()), int*>::value;
    static_assert(std::is_same<decltype(std::move(o1).operator->()), int*>::value);
    CHECK(success);

    struct int_box {
        int i_;
    };
    int_box                                   i1{3};
    beman::optional::optional<int_box&>       ob1 = i1;
    beman::optional::optional<int_box&>       ob2;
    const beman::optional::optional<int_box&> ob3 = i1;

    CHECK(3 == ob1->i_);
    CHECK(3 == ob3->i_);

    success = std::is_same_v<decltype(ob1->i_), int>;
    static_assert(std::is_same_v<decltype(ob1->i_), int>);
    CHECK(success);
    success = std::is_same_v<decltype(ob2->i_), int>;
    static_assert(std::is_same_v<decltype(ob2->i_), int>);
    CHECK(success);
    success = std::is_same_v<decltype(ob3->i_), int>;
    static_assert(std::is_same_v<decltype(ob3->i_), int>);
    CHECK(success);
    success = std::is_same_v<decltype(std::move(ob1)->i_), int>;
    static_assert(std::is_same_v<decltype(std::move(ob1)->i_), int>);
    CHECK(success);
}

TEST_CASE("OptionalRefTest: AmbiguousConversion", "[optional]") {
    struct TypedInt {
        int c;
        TypedInt(int i) : c(i) {}
        operator int() const { return c; }
    };

    TypedInt c{42};

    auto x1 = beman::optional::optional<int>{}.value_or(c);
    auto x2 = beman::optional::optional<TypedInt>{}.value_or(7);

    auto x3 = beman::optional::optional<int&>{}.value_or(c);
    auto x4 = beman::optional::optional<TypedInt&>{}.value_or(7);
    CHECK(42 == x1);
    CHECK(7 == x2);
    CHECK(42 == x3);
    CHECK(7 == x4);
}

TEST_CASE("OptionalRefTest: MoveCheck", "[optional]") {
    int  x = 0;
    int& y = std::move(beman::optional::optional<int&>(x)).value();
    CHECK(&x == &y);

    int& z = *std::move(beman::optional::optional<int&>(x));
    CHECK(&x == &z);
}

TEST_CASE("OptionalRefTest: SwapValue", "[optional]") {
    int                             var    = 42;
    int                             twelve = 12;
    beman::optional::optional<int&> o1     = var;
    beman::optional::optional<int&> o2     = twelve;
    o1.swap(o2);
    CHECK(12 == o1.value());
    CHECK(42 == o2.value());
}

TEST_CASE("OptionalRefTest: SwapWNull", "[optional]") {
    int var = 42;

    beman::optional::optional<int&> o1 = var;
    beman::optional::optional<int&> o2 = beman::optional::nullopt;
    o1.swap(o2);
    CHECK(!o1.has_value());
    CHECK(42 == o2.value());
}

TEST_CASE("OptionalRefTest: SwapNullIntializedWithValue", "[optional]") {
    int                             var = 42;
    beman::optional::optional<int&> o1  = beman::optional::nullopt;
    beman::optional::optional<int&> o2  = var;
    o1.swap(o2);
    CHECK(42 == o1.value());
    CHECK(!o2.has_value());
}

TEST_CASE("OptionalRefTest: AssignFromOptional", "[optional]") {
    int                             var = 42;
    beman::optional::optional<int&> o1  = beman::optional::nullopt;
    beman::optional::optional<int&> o2  = var;

    o2 = o1;

    using beman::optional::tests::base;
    using beman::optional::tests::derived;

    base                             b{1};
    derived                          d(1, 2);
    beman::optional::optional<base&> empty_base;
    beman::optional::optional<base&> engaged_base{b};

    beman::optional::optional<derived&> empty_derived_ref;
    beman::optional::optional<derived&> engaged_derived_ref{d};

    beman::optional::optional<base&> optional_base_ref;
    optional_base_ref = empty_base;
    CHECK(!optional_base_ref.has_value());
    optional_base_ref = engaged_base;
    CHECK(optional_base_ref.has_value());

    optional_base_ref = empty_derived_ref;
    CHECK(!optional_base_ref.has_value());

    optional_base_ref = engaged_derived_ref;
    CHECK(optional_base_ref.has_value());

    beman::optional::optional<derived> empty_derived;
    beman::optional::optional<derived> engaged_derived{d};

    static_assert(std::is_constructible_v<const base&, derived>);

    beman::optional::optional<const base&> optional_base_const_ref;
    optional_base_const_ref = empty_derived;
    CHECK(!optional_base_const_ref.has_value());

    optional_base_const_ref = engaged_derived;
    CHECK(optional_base_const_ref.has_value());

    if (empty_derived) {
        optional_base_ref = empty_derived.value();
    } else {
        optional_base_ref.reset();
    }
    CHECK(!optional_base_ref.has_value());

    if (engaged_derived) {
        optional_base_ref = engaged_derived.value();
    } else {
        optional_base_ref.reset();
    }
    CHECK(optional_base_ref.has_value());

    derived d2(2, 2);
    engaged_derived = d2;
    CHECK(static_cast<base>(d2).m_i == optional_base_ref.value().m_i);

    // deleted the rvalue ref overload
    //     template <class U>
    //        constexpr optional& operator=(optional<U>&& rhs) = delete;
    // -- force failures for
    // optional_base_const_ref = beman::optional::optional<derived>(derived(3, 4));
    // and
    // optional_base_const_ref = [](){return beman::optional::optional<derived>(derived(3, 4));}();
    // TODO: Add to "fail-to-compile" tests when they exist
}

TEST_CASE("OptionalRefTest: ConstructFromOptional", "[optional]") {
    int                             var = 42;
    beman::optional::optional<int&> o1  = beman::optional::nullopt;
    beman::optional::optional<int&> o2{var};
    CHECK(!o1.has_value());
    CHECK(o2.has_value());

    using beman::optional::tests::base;
    using beman::optional::tests::derived;

    base                             b{1};
    derived                          d(1, 2);
    beman::optional::optional<base&> disengaged_base;
    beman::optional::optional<base&> engaged_base{b};
    CHECK(!disengaged_base.has_value());
    CHECK(engaged_base.has_value());

    beman::optional::optional<derived&> disengaged_derived_ref;
    beman::optional::optional<derived&> engaged_derived_ref{d};

    beman::optional::optional<base&> optional_base_ref{disengaged_derived_ref};
    CHECK(!optional_base_ref.has_value());

    beman::optional::optional<base&> optional_base_ref2{engaged_derived_ref};
    CHECK(optional_base_ref2.has_value());

    beman::optional::optional<derived> disengaged_derived;
    beman::optional::optional<derived> engaged_derived{d};

    static_assert(std::is_constructible_v<const base&, derived>);

    beman::optional::optional<const base&> optional_base_const_ref{disengaged_derived};
    CHECK(!optional_base_const_ref.has_value());

    beman::optional::optional<const base&> optional_base_const_ref2{engaged_derived};
    CHECK(optional_base_const_ref2.has_value());
}

TEST_CASE("OptionalRefTest: InPlace", "[optional]") {
    int one      = 1;
    int two      = 2;
    int fortytwo = 42;

    beman::optional::optional<int&> o1{beman::optional::in_place, one};
    beman::optional::optional<int&> o2(beman::optional::in_place, two);
    CHECK(o1);
    CHECK(o1 == 1);
    CHECK(o2);
    CHECK(o2 == 2);

    beman::optional::optional<const int&> o3(beman::optional::in_place, fortytwo);
    CHECK(o3 == 42);

    // beman::optional::optional<std::vector<int>&> o5(beman::optional::in_place, {0, 1});
    // CHECK (o5);
    // CHECK ((*o5)[0] == 0);
    // CHECK ((*o5)[1] == 1);

    // beman::optional::optional<std::tuple<int, int> const&> o4(beman::optional::in_place, zero, one);
    // CHECK (o4);
    // CHECK (std::get<0>(*o4) == 0);
    // CHECK (std::get<1>(*o4) == 1);
}

TEST_CASE("OptionalRefTest: OptionalOfOptional", "[optional]") {
    using O = beman::optional::optional<int>;
    O                             o;
    beman::optional::optional<O&> oo1a(o);
    beman::optional::optional<O&> oo1{o};
    beman::optional::optional<O&> oo1b = o;
    CHECK(oo1.has_value());
    oo1 = o;
    CHECK(oo1.has_value());
    CHECK(&oo1.value() == &o);
    oo1.emplace(o); // emplace, like assignment, binds the reference
    CHECK(oo1.has_value());
    CHECK(&oo1.value() == &o);

    beman::optional::optional<const O&> oo2 = o;
    CHECK(oo2.has_value());
    oo2 = o;
    CHECK(oo2.has_value());
    CHECK(&oo2.value() == &o);
    oo2.emplace(o);
    CHECK(oo2.has_value());
    CHECK(&oo2.value() == &o);
}

TEST_CASE("OptionalRefTest: ConstructFromReferenceWrapper", "[optional]") {
    using O = beman::optional::optional<int>;
    O o;

    beman::optional::optional<O&> oo1 = std::ref(o);
    CHECK(oo1.has_value());
    oo1 = std::ref(o);
    CHECK(oo1.has_value());
    CHECK(&oo1.value() == &o);

    auto                          lvalue_refwrapper = std::ref(o);
    beman::optional::optional<O&> oo2               = lvalue_refwrapper;
    CHECK(oo2.has_value());
    oo2 = lvalue_refwrapper;
    CHECK(oo2.has_value());
    CHECK(&oo2.value() == &o);

    beman::optional::optional<const O&> oo3 = std::ref(o);
    CHECK(oo3.has_value());
    oo3 = std::ref(o);
    CHECK(oo3.has_value());
    CHECK(&oo3.value() == &o);

    beman::optional::optional<const O&> oo4 = lvalue_refwrapper;
    CHECK(oo4.has_value());
    oo4 = lvalue_refwrapper;
    CHECK(oo4.has_value());
    CHECK(&oo4.value() == &o);
}

TEST_CASE("OptionalRefTest: OverloadResolutionChecksDangling", "[optional]") {
    extern int  check_dangling(beman::optional::optional<const std::string&>);
    extern void check_dangling(beman::optional::optional<const char*>);
    std::string lvalue_string = "abc";
    static_assert(std::is_same_v<decltype(check_dangling(lvalue_string)), int>);
    //    static_assert(std::is_same_v<decltype(check_dangling("abc")), void>);
}

// beman::optional::optional<int const&> foo() {
//     beman::optional::optional<int> o(10);
//     return o; // Thanks to a simpler implicit move.
//     /* error: use of deleted function ‘constexpr
//     beman::optional::optional<T&>::optional(beman::optional::optional<U>&&) [with U = int; T = const int]’
//      */
// }

// TEST_CASE("OptionalRefTest: iff", "[optional]") {
//     beman::optional::optional<int const&> o =
//         beman::optional::optional<int>(o);
//     // error: use of deleted function ‘constexpr
//     // beman::optional::optional<T&>::optional(beman::optional::optional<U>&&)
//     // [with U = int; T = const int]’
// }

// TEST_CASE("OptionalRefTest: dangle", "[optional]") {
//     extern int check_dangling(
//         beman::optional::optional<std::string const&>); // #1
//     extern void check_dangling(
//         beman::optional::optional<char const*&>); // #2
//     beman::optional::optional<std::string> optional_string  = "abc";
//     beman::optional::optional<char const*> optional_pointer = "abc";
//     static_assert(std::is_same_v<decltype(check_dangling(optional_string)),
//                                  int>); // unambiguously calls #1
//     static_assert(std::is_same_v<decltype(check_dangling(
//                                      beman::optional::optional<char
//                                      const*&>(
//                                          optional_pointer))),
//                                  void>); // unambiguously calls #2
//     static_assert(std::is_same_v<decltype(check_dangling(optional_pointer)),
//                   void>); // ambiguous
//     // error: call of overloaded
//     // ‘check_dangling(beman::optional::optional<const char*>&)’ is
//     ambiguous
// }
// namespace {
// void process(beman::optional::optional<std::string const&>) {}
// void process(beman::optional::optional<char const* const&>) {}
// }
// TEST_CASE("OptionalRefTest: more_dangle", "[optional]") {

//     char const* cstr = "Text";
//     std::string s       = cstr;
//     process(s);    // Picks, `optional<std::string const&>` overload
//     // process(cstr); // Ambiguous, but only std::optional<char const* const&>
// }
