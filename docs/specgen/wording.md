::: wording

## Optional objects [optional](){.sref} {-}

### General [optional.general](){.sref} {-}

[#]{.pnum} Subclause [optional](){.sref} describes class template `optional` that represents optional objects. An *optional object* is an object that contains the storage for another object and manages the lifetime of this contained object, if any. The contained object may be initialized after the optional object has been initialized, and may be destroyed before the optional object has been destroyed. The initialization state of the contained object is tracked by the optional object.

### Header `<optional>` synopsis [optional.syn](){.sref} {-}

```cpp
namespace std {
  // @[optional.optional](){.sref}@, class template \tcode{optional}

  template<class T> class optional;

  // @[optional.optional.ref](){.sref}@, partial specialization of \tcode{optional} for lvalue reference types

  template<class T> class optional<T&>;

  namespace ranges {
    template<class T> inline constexpr bool ranges::enable_view<optional<T>> = true;

    template<class T>
    inline constexpr bool ranges::enable_borrowed_range<optional<T&>> = true;
  }

  template<class T>
  concept $is-derived-from-optional$ = requires(const T& t) {
    []<class U>(const optional<U>&) {}(t);
  }; // exposition only

  // @[optional.nullopt](){.sref}@, no-value state indicator

  struct nullopt_t{@\seebelow@};

  inline constexpr nullopt_t nullopt{nullopt_t::Tag::tag};

  // @[optional.bad.access](){.sref}@, class \tcode{bad_optional_access}

  class bad_optional_access;

  // @[optional.relops](){.sref}@, relational operators

  template<class T, class U>
  constexpr bool operator==(const optional<T>&, const optional<U>&);

  template<class T, class U>
  constexpr bool operator!=(const optional<T>&, const optional<U>&);

  template<class T, class U>
  constexpr bool operator<(const optional<T>&, const optional<U>&);

  template<class T, class U>
  constexpr bool operator>(const optional<T>&, const optional<U>&);

  template<class T, class U>
  constexpr bool operator<=(const optional<T>&, const optional<U>&);

  template<class T, class U>
  constexpr bool operator>=(const optional<T>&, const optional<U>&);

  template<class T, three_way_comparable_with<T> U>
  constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>&,
                                                         const optional<U>&);

  // @[optional.nullops](){.sref}@, comparison with \tcode{nullopt}

  template<class T> constexpr bool operator==(const optional<T>&, nullopt_t) noexcept;

  template<class T>
  constexpr strong_ordering operator<=>(const optional<T>&, nullopt_t) noexcept;

  // @[optional.comp.with.t](){.sref}@, comparison with \tcode{T}

  template<class T, class U> constexpr bool operator==(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator==(const T&, const optional<U>&);

  template<class T, class U> constexpr bool operator!=(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator!=(const T&, const optional<U>&);

  template<class T, class U> constexpr bool operator<(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator<(const T&, const optional<U>&);

  template<class T, class U> constexpr bool operator>(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator>(const T&, const optional<U>&);

  template<class T, class U> constexpr bool operator<=(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator<=(const T&, const optional<U>&);

  template<class T, class U> constexpr bool operator>=(const optional<T>&, const U&);

  template<class T, class U> constexpr bool operator>=(const T&, const optional<U>&);

  template<class T, class U>
    requires(!$is-derived-from-optional$<U>) && three_way_comparable_with<T, U>
  constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>&, const U&);

  // @[optional.specalg](){.sref}@, specialized algorithms

  template<class T>
  constexpr void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)));

  template<int = 0, class T>
  constexpr optional<decay_t<T>> make_optional(T&&) noexcept(
      is_nothrow_constructible_v<optional<decay_t<T>>, T>);

  template<class T, class... Args>
  constexpr optional<T> make_optional(Args&&...) noexcept(
      is_nothrow_constructible_v<T, Args...>);

  template<class T, class U, class... Args>
  constexpr optional<T> make_optional(initializer_list<U>, Args&&...) noexcept(
      is_nothrow_constructible_v<T, initializer_list<U>&, Args...>);

  // @[optional.hash](){.sref}@, hash support
}
```

### Class `bad_optional_access` [optional.bad.access](){.sref} {-}

```cpp
namespace std {
  class bad_optional_access : public exception {
  public:
    bad_optional_access() = default;

    const char* what() const noexcept;
  };
}
```

[#]{.pnum} The class `bad_optional_access` defines the type of objects thrown as exceptions to report the situation where an attempt is made to access the value of an optional object that does not contain a value.

```cpp
const char* what() const noexcept;
```

[#]{.pnum} *Returns*: An implementation-defined NTBS, which during constant evaluation is encoded with the ordinary literal encoding ([lex.ccon](){.sref}).

### Class template `optional` [optional.optional](){.sref} {-}

```cpp
namespace std {
  template<class T, class W>
  constexpr bool $converts-from-any-cvref$ = disjunction_v<
      is_constructible<T, W&>, is_convertible<W&, T>, is_constructible<T, W>,
      is_convertible<W, T>, is_constructible<T, const W&>, is_convertible<const W&, T>,
      is_constructible<T, const W>, is_convertible<const W, T>>; // exposition only
}
```

[#]{.pnum} The exposition-only variable template `converts-from-any-cvref` is used by some constructors for `optional`.

#### General [optional.optional.general](){.sref} {-}

```cpp
namespace std {
  template<class T>
  class optional {
  public:
    using value_type = T;

    using iterator = $implementation-defined$; // see~@[optional.iterators](){.sref}@

    using const_iterator = $implementation-defined$; // see~@[optional.iterators](){.sref}@

    // @[optional.ctor](){.sref}@, constructors
    constexpr optional() noexcept;

    constexpr optional(nullopt_t) noexcept;

    constexpr optional(const optional&);

    constexpr optional(optional&&) noexcept($see below$);

    template<class... Args> constexpr explicit optional(in_place_t, Args&&...);

    template<class U, class... Args>
    constexpr explicit optional(in_place_t, initializer_list<U>, Args&&...);

    template<class U = T> constexpr explicit($see below$) optional(U&&);

    template<class U> constexpr explicit($see below$) optional(const optional<U>&);

    template<class U> constexpr explicit($see below$) optional(optional<U>&&);

    // @[optional.dtor](){.sref}@, destructor

    constexpr ~optional();

    // @[optional.assign](){.sref}@, assignment
    constexpr optional& operator=(nullopt_t) noexcept;

    constexpr optional& operator=(const optional&);

    constexpr optional& operator=(optional&&) noexcept($see below$);

    template<class U = T> constexpr optional& operator=(U&&);

    template<class U> constexpr optional& operator=(const optional<U>&);

    template<class U> constexpr optional& operator=(optional<U>&&);
    template<class... Args> constexpr T& emplace(Args&&...);
    template<class U, class... Args>
    constexpr T& emplace(initializer_list<U>, Args&&...);

    // @[optional.swap](){.sref}@, swap
    constexpr void swap(optional&) noexcept($see below$);

    // @[optional.iterators](){.sref}@, iterator support
    constexpr iterator begin() noexcept;

    constexpr const_iterator begin() const noexcept;

    constexpr iterator end() noexcept;

    constexpr const_iterator end() const noexcept;

    // @[optional.observe](){.sref}@, observers
    constexpr const T* operator->() const;

    constexpr T* operator->();
    constexpr T& operator*() &;

    constexpr const T& operator*() const&;

    constexpr T&& operator*() &&;

    constexpr explicit operator bool() const noexcept;

    constexpr bool has_value() const noexcept;

    constexpr T& value() &;

    constexpr const T& value() const&;

    constexpr T&& value() &&;

    template<class U = remove_cv_t<T>> constexpr remove_cv_t<T> value_or(U&&) const&;
    template<class U = remove_cv_t<T>> constexpr remove_cv_t<T> value_or(U&&) &&;

    // @[optional.monadic](){.sref}@, monadic operations

    template<class F> constexpr auto and_then(F&&) &;

    template<class F> constexpr auto and_then(F&&) &&;

    template<class F> constexpr auto and_then(F&&) const&;

    template<class F> constexpr auto and_then(F&&) const&&;

    template<class F> constexpr auto transform(F&&) &;

    template<class F> constexpr auto transform(F&&) &&;

    template<class F> constexpr auto transform(F&&) const&;

    template<class F> constexpr auto transform(F&&) const&&;

    template<class F> constexpr optional or_else(F&&) const&;

    template<class F> constexpr optional or_else(F&&) &&;

    // @[optional.mod](){.sref}@, modifiers
    constexpr void reset() noexcept;

  private:
    union {
      T $val$; // exposition only
    };
  };
}
```

[#]{.pnum} An instance of `optional<T>` is said to *contain a value* when and only when its member `$val$` is active ([class.union.general](){.sref}); `$val$` is referred to as its *contained value*. An optional object's contained value is nested within ([intro.object](){.sref}) the optional object.

[#]{.pnum} A type `X` is a *valid contained type* for `optional` if `X` is an lvalue reference type or a complete non-array object type, and `remove_cvref_t<X>` is a type other than `in_place_t` or `nullopt_t`. If a specialization of `optional` is instantiated with a type `T` that is not a valid contained type for `optional`, the program is ill-formed. If `T` is an object type, `T` shall meet the *Cpp17Destructible* requirements ([Table \[cpp17.destructible\]](#cpp17.destructible)).

#### Constructors [optional.ctor](){.sref} {-}

```cpp
constexpr optional() noexcept;
constexpr optional(nullopt_t) noexcept;
```

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

[#]{.pnum} *Remarks*: No contained value is initialized. For every object type `T` these constructors are constexpr constructors ([dcl.constexpr](){.sref}).

```cpp
constexpr optional(const optional& rhs);
```

[#]{.pnum} *Effects*: If `rhs` contains a value, direct-non-list-initializes `$val$` with `rhs.$val$`.

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: This constructor is defined as deleted unless `is_copy_constructible_v<T>` is `true`. If `is_trivially_copy_constructible_v<T>` is `true`, this constructor is trivial.

```cpp
constexpr optional(optional&& rhs) noexcept($see below$);
```

[#]{.pnum} *Constraints*: `is_move_constructible_v<T>` is `true`.

[#]{.pnum} *Effects*: If `rhs` contains a value, direct-non-list-initializes `$val$` with `std::move(rhs.$val$)`. `rhs.has_value()` is unchanged.

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: The exception specification is equivalent to `is_nothrow_move_constructible_v<T>`. If `is_trivially_move_constructible_v<T>` is `true`, this constructor is trivial.

```cpp
template<class... Args>
  constexpr explicit optional(in_place_t, Args&&... args);
```

[#]{.pnum} *Constraints*: `is_constructible_v<T, Args...>` is `true`.

[#]{.pnum} *Effects*: Direct-non-list-initializes `$val$` with `std::forward<Args>(args)...`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: If `T`'s constructor selected for the initialization is a constexpr constructor, this constructor is a constexpr constructor.

```cpp
template<class U, class... Args>
  constexpr explicit optional(in_place_t, initializer_list<U> il, Args&&... args);
```

[#]{.pnum} *Constraints*: `is_constructible_v<T, initializer_list<U>&, Args&&...>` is `true`.

[#]{.pnum} *Effects*: Direct-non-list-initializes `$val$` with `il, std::forward<Args>(args)...`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: If `T`'s constructor selected for the initialization is a constexpr constructor, this constructor is a constexpr constructor.

```cpp
template<class U = T>
  constexpr explicit($see below$) optional(U&& v);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_constructible_v<T, U>` is `true`,
- [#.#]{.pnum} `is_same_v<remove_cvref_t<U>, in_place_t>` is `false`, and
- [#.#]{.pnum} `is_same_v<remove_cvref_t<U>, optional>` is `false`.

[#]{.pnum} *Effects*: Direct-non-list-initializes `$val$` with `std::forward<U>(v)`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: If `T`'s selected constructor is a constexpr constructor, this constructor is a constexpr constructor. The expression inside `explicit` is equivalent to:

```cpp
!is_convertible_v<U, T>
```

```cpp
template<class U>
  constexpr explicit($see below$) optional(const optional<U>& rhs);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<T, U>` is `false`,
- [#.#]{.pnum} `is_constructible_v<T, const U&>` is `true`, and
- [#.#]{.pnum} `converts-from-any-cvref<T, optional<U>>` is `false`.

[#]{.pnum} *Effects*: If `rhs` contains a value, direct-non-list-initializes `$val$` with `rhs.operator*()`.

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: The expression inside `explicit` is equivalent to:

```cpp
!is_convertible_v<U, T>
```

```cpp
template<class U>
  constexpr explicit($see below$) optional(optional<U>&& rhs);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<T, U>` is `false`,
- [#.#]{.pnum} `is_constructible_v<T, U>` is `true`, and
- [#.#]{.pnum} `converts-from-any-cvref<T, optional<U>>` is `false`.

[#]{.pnum} *Effects*: If `rhs` contains a value, direct-non-list-initializes `$val$` with `std::move(rhs).operator*()`. `rhs.has_value()` is unchanged.

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: The expression inside `explicit` is equivalent to:

```cpp
!is_convertible_v<U, T>
```

#### Destructor [optional.dtor](){.sref} {-}

```cpp
constexpr ~optional();
```

[#]{.pnum} *Effects*: If `is_trivially_destructible_v<T>` is `false` and `*this` contains a value, calls `$val$.T::~T()`.

[#]{.pnum} *Remarks*: If `is_trivially_destructible_v<T>` is `true`, then this destructor is trivial.

#### Assignment [optional.assign](){.sref} {-}

```cpp
constexpr optional& operator=(nullopt_t) noexcept;
```

[#]{.pnum} *Effects*: If `*this` contains a value, calls `$val$.T::~T()` to destroy the contained value; otherwise no effect.

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

[#]{.pnum} *Returns*: `*this`.

```cpp
constexpr optional& operator=(const optional& rhs);
```

[#]{.pnum} *Effects*: See [Table \[optional.assign.copy\]](#optional.assign.copy).

| | `*this` contains a value | `*this` does not contain a value |
|---|---|---|
| **`rhs` contains a value** | assigns `rhs.$val$` to `$val$` | direct-non-list-initializes `$val$` with `rhs.$val$` |
| **`rhs` does not contain a value** | destroys the contained value by calling `$val$.T::~T()` | no effect |
: [`optional::operator=(const optional&)` effects]{#optional.assign.copy}

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Returns*: `*this`.

[#]{.pnum} *Remarks*: If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an exception is thrown during the call to `T`'s copy constructor, no effect. If an exception is thrown during the call to `T`'s copy assignment, the state of its contained value is as defined by the exception safety guarantee of `T`'s copy assignment. This operator is defined as deleted unless `is_copy_constructible_v<T>` is `true` and `is_copy_assignable_v<T>` is `true`. If `is_trivially_copy_constructible_v<T> &&` `is_trivially_copy_assignable_v<T> &&` `is_trivially_destructible_v<T>` is `true`, this assignment operator is trivial.

```cpp
constexpr optional& operator=(optional&& rhs) noexcept($see below$);
```

[#]{.pnum} *Constraints*: `is_move_constructible_v<T>` is `true` and `is_move_assignable_v<T>` is `true`.

[#]{.pnum} *Effects*: See [Table \[optional.assign.move\]](#optional.assign.move). The result of the expression `rhs.has_value()` remains unchanged.

| | `*this` contains a value | `*this` does not contain a value |
|---|---|---|
| **`rhs` contains a value** | assigns `std::move(rhs.$val$)` to `$val$` | direct-non-list-initializes `$val$` with `std::move(rhs.$val$)` |
| **`rhs` does not contain a value** | destroys the contained value by calling `$val$.T::~T()` | no effect |
: [`optional::operator=(optional&&)` effects]{#optional.assign.move}

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Returns*: `*this`.

[#]{.pnum} *Remarks*: The exception specification is equivalent to:

```cpp
is_nothrow_move_constructible_v<T>
```

[#]{.pnum} If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an exception is thrown during the call to `T`'s move constructor, the state of `rhs.$val$` is determined by the exception safety guarantee of `T`'s move constructor. If an exception is thrown during the call to `T`'s move assignment, the states of `$val$` and `rhs.$val$` are determined by the exception safety guarantee of `T`'s move assignment. If `is_trivially_move_constructible_v<T> &&` `is_trivially_move_assignable_v<T> &&` `is_trivially_destructible_v<T>` is `true`, this assignment operator is trivial.

```cpp
template<class U = T>
  constexpr optional& operator=(U&& v);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cvref_t<U>, optional>` is `false`,
- [#.#]{.pnum} `conjunction_v<is_scalar<T>, is_same<T, decay_t<U>>>` is `false`,
- [#.#]{.pnum} `is_constructible_v<T, U>` is `true`, and
- [#.#]{.pnum} `is_assignable_v<T&, U>` is `true`.

[#]{.pnum} *Effects*: If `*this` contains a value, assigns `std::forward<U>(v)` to `$val$`; otherwise direct-non-list-initializes `$val$` with `std::forward<U>(v)`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Returns*: `*this`.

[#]{.pnum} *Remarks*: If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an exception is thrown during the call to `T`'s constructor, the state of `v` is determined by the exception safety guarantee of `T`'s constructor. If an exception is thrown during the call to `T`'s assignment, the states of `$val$` and `v` are determined by the exception safety guarantee of `T`'s assignment.

```cpp
template<class U>
  constexpr optional& operator=(const optional<U>& rhs);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_constructible_v<T, const U&>` is `true`,
- [#.#]{.pnum} `is_assignable_v<T&, const U&>` is `true`,
- [#.#]{.pnum} `converts-from-any-cvref<T, optional<U>>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, optional<U>&>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, optional<U>&&>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, const optional<U>&>` is `false`, and
- [#.#]{.pnum} `is_assignable_v<T&, const optional<U>&&>` is `false`.

[#]{.pnum} *Effects*: See [Table \[optional.assign.copy.templ\]](#optional.assign.copy.templ).

| | `*this` contains a value | `*this` does not contain a value |
|---|---|---|
| **`rhs` contains a value** | assigns `rhs.operator*()` to `$val$` | direct-non-list-initializes `$val$` with `rhs.operator*()` |
| **`rhs` does not contain a value** | destroys the contained value by calling `$val$.T::~T()` | no effect |
: [`optional::operator=(const optional<U>&)` effects]{#optional.assign.copy.templ}

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Returns*: `*this`.

[#]{.pnum} *Remarks*: If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an exception is thrown during the call to `T`'s constructor, the state of `rhs.$val$` is determined by the exception safety guarantee of `T`'s constructor. If an exception is thrown during the call to `T`'s assignment, the states of `$val$` and `rhs.$val$` are determined by the exception safety guarantee of `T`'s assignment.

```cpp
template<class U>
  constexpr optional& operator=(optional<U>&& rhs);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_constructible_v<T, U>` is `true`,
- [#.#]{.pnum} `is_assignable_v<T&, U>` is `true`,
- [#.#]{.pnum} `converts-from-any-cvref<T, optional<U>>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, optional<U>&>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, optional<U>&&>` is `false`,
- [#.#]{.pnum} `is_assignable_v<T&, const optional<U>&>` is `false`, and
- [#.#]{.pnum} `is_assignable_v<T&, const optional<U>&&>` is `false`.

[#]{.pnum} *Effects*: See [Table \[optional.assign.move.templ\]](#optional.assign.move.templ). The result of the expression `rhs.has_value()` remains unchanged.

| | `*this` contains a value | `*this` does not contain a value |
|---|---|---|
| **`rhs` contains a value** | assigns `std::move(rhs).operator*()` to `$val$` | direct-non-list-initializes `$val$` with `std::move(rhs).operator*()` |
| **`rhs` does not contain a value** | destroys the contained value by calling `$val$.T::~T()` | no effect |
: [`optional::operator=(optional<U>&&)` effects]{#optional.assign.move.templ}

[#]{.pnum} *Postconditions*: `rhs.has_value() == this->has_value()`.

[#]{.pnum} *Returns*: `*this`.

[#]{.pnum} *Remarks*: If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an exception is thrown during the call to `T`'s constructor, the state of `rhs.$val$` is determined by the exception safety guarantee of `T`'s constructor. If an exception is thrown during the call to `T`'s assignment, the states of `$val$` and `rhs.$val$` are determined by the exception safety guarantee of `T`'s assignment.

```cpp
template<class... Args>
  constexpr T& emplace(Args&&... args);
```

[#]{.pnum} *Mandates*: `is_constructible_v<T, Args&&...>` is `true`.

[#]{.pnum} *Effects*: Calls `*this = nullopt`. Then direct-non-list-initializes `$val$` with `std::forward<Args>(args)...`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Returns*: `$val$`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: If an exception is thrown during the call to `T`'s constructor, `*this` does not contain a value, and the previous `$val$` (if any) has been destroyed.

```cpp
template<class U, class... Args>
  constexpr T& emplace(initializer_list<U> il, Args&&... args);
```

[#]{.pnum} *Mandates*: `is_constructible_v<T, initializer_list<U>&, Args&&...>` is `true`.

[#]{.pnum} *Effects*: Calls `*this = nullopt`. Then direct-non-list-initializes `$val$` with `il, std::forward<Args>(args)...`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Returns*: `$val$`.

[#]{.pnum} *Throws*: Any exception thrown by the selected constructor of `T`.

[#]{.pnum} *Remarks*: If an exception is thrown during the call to `T`'s constructor, `*this` does not contain a value, and the previous `$val$` (if any) has been destroyed.

#### Swap [optional.swap](){.sref} {-}

```cpp
constexpr void swap(optional& rhs) noexcept($see below$);
```

[#]{.pnum} *Mandates*: `is_move_constructible_v<T>` is `true`.

[#]{.pnum} *Preconditions*: `T` meets the *Cpp17Swappable* requirements ([swappable.requirements](){.sref}).

[#]{.pnum} *Effects*: See [Table \[optional.swap\]](#optional.swap).

| | `*this` contains a value | `*this` does not contain a value |
|---|---|---|
| **`rhs` contains a value** | calls `swap($val$, rhs.$val$)` | direct-non-list-initializes `$val$` with `std::move(rhs.$val$)`, followed by `rhs.$val$.T::~T()`; postcondition is that `*this` contains a value and `rhs` does not contain a value |
| **`rhs` does not contain a value** | direct-non-list-initializes `rhs.$val$` with `std::move($val$)`, followed by `$val$.T::~T()`; postcondition is that `*this` does not contain a value and `rhs` contains a value | no effect |
: [`optional::swap(optional&)` effects]{#optional.swap}

[#]{.pnum} *Throws*: Any exceptions thrown by the operations in the relevant part of [Table \[optional.swap\]](#optional.swap).

[#]{.pnum} *Remarks*: The exception specification is equivalent to:

```cpp
is_nothrow_move_constructible_v<T> && is_nothrow_swappable_v<T>
```

[#]{.pnum} If any exception is thrown, the results of the expressions `this->has_value()` and `rhs.has_value()` remain unchanged. If an exception is thrown during the call to function `swap`, the states of `$val$` and `rhs.$val$` are determined by the exception safety guarantee of `swap` for lvalues of `T`. If an exception is thrown during the call to `T`'s move constructor, the states of `$val$` and `rhs.$val$` are determined by the exception safety guarantee of `T`'s move constructor.

#### Iterator support [optional.iterators](){.sref} {-}

```cpp
using iterator = $implementation-defined$;
using const_iterator = $implementation-defined$;
```

[#]{.pnum} These types model `contiguous_iterator` ([iterator.concept.contiguous](){.sref}), meet the *Cpp17RandomAccessIterator* requirements ([random.access.iterators](){.sref}), and meet the requirements for constexpr iterators ([iterator.requirements.general](){.sref}), with value type `remove_cv_t<T>`. The reference type is `T&` for `iterator` and `const T&` for `const_iterator`.

[#]{.pnum} All requirements on container iterators ([container.reqmts](){.sref}) apply to `optional::iterator` and `optional::const_iterator` as well.

[#]{.pnum} Any operation that initializes or destroys the contained value of an optional object invalidates all iterators into that object.

```cpp
constexpr iterator begin() noexcept;
constexpr const_iterator begin() const noexcept;
```

[#]{.pnum} *Returns*: If `has_value()` is `true`, an iterator referring to the contained value. Otherwise, a past-the-end iterator value.

```cpp
constexpr iterator end() noexcept;
constexpr const_iterator end() const noexcept;
```

[#]{.pnum} *Returns*: `begin() + has_value()`.

#### Observers [optional.observe](){.sref} {-}

```cpp
constexpr const T* operator->() const;
constexpr T* operator->();
```

[#]{.pnum} *Hardened preconditions*: `has_value()` is `true`.

[#]{.pnum} *Returns*: `addressof($val$)`.

[#]{.pnum} *Remarks*: These functions are constexpr functions.

```cpp
constexpr T& operator*() &;
constexpr const T& operator*() const&;
```

[#]{.pnum} *Hardened preconditions*: `has_value()` is `true`.

[#]{.pnum} *Returns*: `$val$`.

[#]{.pnum} *Remarks*: These functions are constexpr functions.

```cpp
constexpr T&& operator*() &&;
```

[#]{.pnum} *Hardened preconditions*: `has_value()` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return std::move($val$);
```

```cpp
constexpr explicit operator bool() const noexcept;
```

[#]{.pnum} *Returns*: `true` if and only if `*this` contains a value.

[#]{.pnum} *Remarks*: This function is a constexpr function.

```cpp
constexpr bool has_value() const noexcept;
```

[#]{.pnum} *Returns*: `true` if and only if `*this` contains a value.

[#]{.pnum} *Remarks*: This function is a constexpr function.

```cpp
constexpr T& value() &;
constexpr const T& value() const&;
```

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return has_value() ? $val$ : throw bad_optional_access();
```

```cpp
constexpr T&& value() &&;
```

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return has_value() ? std::move($val$) : throw bad_optional_access();
```

```cpp
template<class U = remove_cv_t<T>>
  constexpr remove_cv_t<T> value_or(U&& v) const&;
```

[#]{.pnum} *Mandates*: `is_convertible_v<const T&, X>` is `true` and `is_convertible_v<U, X>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
using X = remove_cv_t<T>;

if (has_value())
  return $val$;
return std::forward<U>(v);
```

```cpp
template<class U = remove_cv_t<T>>
  constexpr remove_cv_t<T> value_or(U&& v) &&;
```

[#]{.pnum} *Mandates*: `is_convertible_v<T, X>` is `true` and `is_convertible_v<U, X>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
using X = remove_cv_t<T>;

if (has_value()) {
  return std::move($val$);
}
return std::forward<U>(v);
```

#### Monadic operations [optional.monadic](){.sref} {-}

```cpp
template<class F>
  constexpr auto and_then(F&& f) &;
template<class F>
  constexpr auto and_then(F&& f) const&;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, decltype(($val$))>`.

[#]{.pnum} *Mandates*: `remove_cvref_t<U>` is a specialization of `optional`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (*this) {
  return invoke(std::forward<F>(f), $val$);
} else {
  return remove_cvref_t<U>();
}
```

```cpp
template<class F>
  constexpr auto and_then(F&& f) &&;
template<class F>
  constexpr auto and_then(F&& f) const&&;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, decltype(std::move($val$))>`.

[#]{.pnum} *Mandates*: `remove_cvref_t<U>` is a specialization of `optional`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (*this) {
  return invoke(std::forward<F>(f), std::move($val$));
} else {
  return remove_cvref_t<U>();
}
```

```cpp
template<class F>
  constexpr auto transform(F&& f) &;
template<class F>
  constexpr auto transform(F&& f) const&;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, decltype(($val$))>`.

[#]{.pnum} *Mandates*: `U` is a valid contained type for `optional`. The declaration

```cpp
U u(invoke(std::forward<F>(f), $val$));
```

is well-formed for some invented variable `u`.

::: note

There is no requirement that `U` is movable ([dcl.init.general](){.sref}).

:::

[#]{.pnum} *Returns*: If `*this` contains a value, an `optional<U>` object whose contained value is direct-non-list-initialized with `invoke(std::forward<F>(f), $val$)`; otherwise, `optional<U>()`.

```cpp
template<class F>
  constexpr auto transform(F&& f) &&;
template<class F>
  constexpr auto transform(F&& f) const&&;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, decltype(std::move($val$))>`.

[#]{.pnum} *Mandates*: `U` is a valid contained type for `optional`. The declaration

```cpp
U u(invoke(std::forward<F>(f), std::move($val$)));
```

is well-formed for some invented variable `u`.

::: note

There is no requirement that `U` is movable ([dcl.init.general](){.sref}).

:::

[#]{.pnum} *Returns*: If `*this` contains a value, an `optional<U>` object whose contained value is direct-non-list-initialized with `invoke(std::forward<F>(f), std::move($val$))`; otherwise, `optional<U>()`.

```cpp
template<class F>
  constexpr optional or_else(F&& f) const&;
```

[#]{.pnum} *Constraints*: `F` models `invocable` and `T` models `copy_constructible`.

[#]{.pnum} *Mandates*: `is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (has_value())
  return $val$;

return std::forward<F>(f)();
```

```cpp
template<class F>
  constexpr optional or_else(F&& f) &&;
```

[#]{.pnum} *Constraints*: `F` models `invocable` and `T` models `move_constructible`.

[#]{.pnum} *Mandates*: `is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (has_value())
  return std::move($val$);

return std::forward<F>(f)();
```

#### Modifiers [optional.mod](){.sref} {-}

```cpp
constexpr void reset() noexcept;
```

[#]{.pnum} *Effects*: If `*this` contains a value, calls `$val$.T::~T()` to destroy the contained value; otherwise no effect.

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

### Partial specialization of `optional` for reference types [optional.optional.ref](){.sref} {-}

#### General [optional.optional.ref.general](){.sref} {-}

```cpp
namespace std {
  template<class T>
  class optional<T&> {
  public:
    using value_type = T;

    using iterator = $implementation-defined$; // see~@[optional.ref.iterators](){.sref}@

    // @[optional.ref.ctor](){.sref}@, constructors

    constexpr optional() noexcept = default;

    constexpr optional(nullopt_t) noexcept;

    constexpr optional(const optional&) noexcept = default;

    template<class Arg> constexpr explicit optional(in_place_t, Arg&&);

    template<class U>
    constexpr explicit(!is_convertible_v<U, T&>)
        optional(U&&) noexcept(is_nothrow_constructible_v<T&, U>);

    template<class U>
    constexpr explicit(!is_convertible_v<U&, T&>)
        optional(optional<U>&) noexcept(is_nothrow_constructible_v<T&, U&>);

    template<class U>
    constexpr explicit(!is_convertible_v<const U&, T&>)
        optional(const optional<U>&) noexcept(is_nothrow_constructible_v<T&, const U&>);

    template<class U>
    constexpr explicit(!is_convertible_v<U, T&>)
        optional(optional<U>&&) noexcept(noexcept(is_nothrow_constructible_v<T&, U>));

    template<class U>
    constexpr explicit(!is_convertible_v<const U, T&>)
        optional(const optional<U>&&) noexcept(
            noexcept(is_nothrow_constructible_v<T&, const U>));

    constexpr ~optional() = default;

    // @[optional.ref.assign](){.sref}@, assignment
    constexpr optional& operator=(nullopt_t) noexcept;

    constexpr optional& operator=(const optional&) noexcept = default;

    template<class U>
    constexpr T& emplace(U&&) noexcept(is_nothrow_constructible_v<T&, U>);

    // @[optional.ref.swap](){.sref}@, swap
    constexpr void swap(optional&) noexcept;

    // @[optional.ref.iterators](){.sref}@, iterator support
    constexpr iterator begin() const noexcept;

    constexpr iterator end() const noexcept;

    // @[optional.ref.observe](){.sref}@, observers
    constexpr T* operator->() const noexcept;

    constexpr T& operator*() const noexcept;

    constexpr explicit operator bool() const noexcept;

    constexpr bool has_value() const noexcept;

    constexpr T& value() const;

    template<class U = remove_cv_t<T>> constexpr decay_t<T> value_or(U&&) const;

    // @[optional.ref.monadic](){.sref}@, monadic operations
    template<class F> constexpr auto and_then(F&&) const;

    template<class F> constexpr optional<invoke_result_t<F, T&>> transform(F&&) const;

    template<class F> constexpr optional or_else(F&&) const;

    // @[optional.ref.mod](){.sref}@, modifiers
    constexpr void reset() noexcept;

  private:
    T* $val$ = nullptr; // exposition only

    // @[optional.ref.expos](){.sref}@, exposition only helper functions
    template<class U> constexpr void $convert-ref-init-val$(U&&); // exposition only
  };
}
```

[#]{.pnum} An object of type `optional<T&>` *contains a value* if and only if `$val$ != nullptr` is `true`. When an `optional<T&>` contains a value, the *contained value* is a reference to `*$val$`.

[#]{.pnum} Each type `optional<T&>` is a trivially copyable class ([class.prop](){.sref}).

#### Constructors [optional.ref.ctor](){.sref} {-}

```cpp
constexpr optional(nullopt_t) noexcept;
```

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

```cpp
template<class Arg>
  constexpr explicit optional(in_place_t, Arg&& arg);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_constructible_v<T&, Arg>` is `true`, and
- [#.#]{.pnum} `reference_constructs_from_temporary_v<T&, Arg>` is `false`.

[#]{.pnum} *Effects*: Equivalent to: `$convert-ref-init-val$(std::forward<Arg>(arg))`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

```cpp
template<class U>
  constexpr explicit(!is_convertible_v<U, T&>) optional(U&& u) noexcept(is_nothrow_constructible_v<T&, U>);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cvref_t<U>, optional>` is `false`,
- [#.#]{.pnum} `is_same_v<remove_cvref_t<U>, in_place_t>` is `false`, and
- [#.#]{.pnum} `is_constructible_v<T&, U>` is `true`.

[#]{.pnum} *Effects*: Equivalent to: `$convert-ref-init-val$(std::forward<U>(u))`.

[#]{.pnum} *Postconditions*: `*this` contains a value.

[#]{.pnum} *Remarks*: This constructor is defined as deleted if

```cpp
reference_constructs_from_temporary_v<T&, U>
```

is `true`.

```cpp
template<class U>
  constexpr explicit(!is_convertible_v<U&, T&>) optional(optional<U>& rhs) noexcept(is_nothrow_constructible_v<T&, U&>);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
- [#.#]{.pnum} `is_same_v<T&, U>` is `false`, and
- [#.#]{.pnum} `is_constructible_v<T&, U&>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (rhs.has_value()) $convert-ref-init-val$(rhs.operator*());
```

[#]{.pnum} *Remarks*: This constructor is defined as deleted if

```cpp
reference_constructs_from_temporary_v<T&, U&>
```

is `true`.

```cpp
template<class U>
  constexpr explicit(!is_convertible_v<const U&, T&>)
      optional(const optional<U>& rhs) noexcept(is_nothrow_constructible_v<T&, const U&>);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
- [#.#]{.pnum} `is_same_v<T&, U>` is `false`, and
- [#.#]{.pnum} `is_constructible_v<T&, const U&>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (rhs.has_value()) $convert-ref-init-val$(rhs.operator*());
```

[#]{.pnum} *Remarks*: This constructor is defined as deleted if

```cpp
reference_constructs_from_temporary_v<T&, const U&>
```

is `true`.

```cpp
template<class U>
  constexpr explicit(!is_convertible_v<U, T&>)
      optional(optional<U>&& rhs) noexcept(noexcept(is_nothrow_constructible_v<T&, U>));
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
- [#.#]{.pnum} `is_same_v<T&, U>` is `false`, and
- [#.#]{.pnum} `is_constructible_v<T&, U>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (rhs.has_value()) $convert-ref-init-val$(std::move(rhs).operator*());
```

[#]{.pnum} *Remarks*: This constructor is defined as deleted if

```cpp
reference_constructs_from_temporary_v<T&, U>
```

is `true`.

```cpp
template<class U>
  constexpr explicit(!is_convertible_v<const U, T&>)
      optional(const optional<U>&& rhs) noexcept(noexcept(is_nothrow_constructible_v<T&, const U>));
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
- [#.#]{.pnum} `is_same_v<T&, U>` is `false`, and
- [#.#]{.pnum} `is_constructible_v<T&, const U>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (rhs.has_value()) $convert-ref-init-val$(std::move(rhs).operator*());
```

[#]{.pnum} *Remarks*: This constructor is defined as deleted if

```cpp
reference_constructs_from_temporary_v<T&, const U>
```

is `true`.

#### Assignment [optional.ref.assign](){.sref} {-}

```cpp
constexpr optional& operator=(nullopt_t) noexcept;
```

[#]{.pnum} *Effects*: Assigns `nullptr` to `$val$`.

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

[#]{.pnum} *Returns*: `*this`.

```cpp
template<class U>
  constexpr T& emplace(U&& u) noexcept(is_nothrow_constructible_v<T&, U>);
```

[#]{.pnum} *Constraints*:

- [#.#]{.pnum} `is_constructible_v<T&, U>` is `true`, and
- [#.#]{.pnum} `reference_constructs_from_temporary_v<T&, U>` is `false`.

[#]{.pnum} *Effects*: Equivalent to: `$convert-ref-init-val$(std::forward<U>(u))`.

[#]{.pnum} *Returns*: `*$val$`.

#### Swap [optional.ref.swap](){.sref} {-}

```cpp
constexpr void swap(optional& rhs) noexcept;
```

[#]{.pnum} *Effects*: Equivalent to: `std::swap($val$, rhs.$val$)`.

#### Iterator support [optional.ref.iterators](){.sref} {-}

```cpp
using iterator = $implementation-defined$;
```

[#]{.pnum} This type models `contiguous_iterator` ([iterator.concept.contiguous](){.sref}), meets the *Cpp17RandomAccessIterator* requirements ([random.access.iterators](){.sref}), and meets the requirements for constexpr iterators ([iterator.requirements.general](){.sref}), with value type `remove_cv_t<T>`. The reference type is `T&` for `iterator`.

[#]{.pnum} All requirements on container iterators ([container.reqmts](){.sref}) apply to `optional::iterator`.

```cpp
constexpr iterator begin() const noexcept;
```

[#]{.pnum} *Returns*: An object `i` of type `iterator`, such that `i` is an iterator referring to `*$val$` if `has_value()` is `true`, and a past-the-end iterator value otherwise.

```cpp
constexpr iterator end() const noexcept;
```

[#]{.pnum} *Returns*: `begin() + has_value()`.

#### Observers [optional.ref.observe](){.sref} {-}

```cpp
constexpr T* operator->() const noexcept;
```

[#]{.pnum} *Hardened preconditions*: `has_value()` is `true`.

[#]{.pnum} *Returns*: `$val$`.

```cpp
constexpr T& operator*() const noexcept;
```

[#]{.pnum} *Hardened preconditions*: `has_value()` is `true`.

[#]{.pnum} *Returns*: `*$val$`.

```cpp
constexpr explicit operator bool() const noexcept;
```

[#]{.pnum} *Returns*: `$val$ != nullptr`.

```cpp
constexpr bool has_value() const noexcept;
```

[#]{.pnum} *Returns*: `$val$ != nullptr`.

```cpp
constexpr T& value() const;
```

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (has_value()) {
  return *$val$;
}
throw bad_optional_access();
```

```cpp
template<class U = remove_cv_t<T>>
  constexpr decay_t<T> value_or(U&& u) const;
```

[#]{.pnum} *Constraints*: `T` is a non-array object type.

[#]{.pnum} *Mandates*: `is_convertible_v<T&, X>` is `true` and `is_convertible_v<U, X>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
using X = remove_cv_t<T>;

if (has_value()) {
  return *$val$;
}
return std::forward<U>(u);
```

[#]{.pnum} *Remarks*: The return type is unspecified if `T` is an array type or a non-object type.

::: note

This is to avoid the declaration being ill-formed.

:::

#### Monadic operations [optional.ref.monadic](){.sref} {-}

```cpp
template<class F>
  constexpr auto and_then(F&& f) const;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, T&>`.

[#]{.pnum} *Mandates*: `U` is a specialization of `optional`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (has_value()) {
  return invoke(std::forward<F>(f), *$val$);
} else {
  return remove_cvref_t<U>();
}
```

```cpp
template<class F>
  constexpr optional<invoke_result_t<F, T&>> transform(F&& f) const;
```

[#]{.pnum} Let `U` be `invoke_result_t<F, T&>`.

[#]{.pnum} *Mandates*: `U` is a valid contained type for `optional`. The declaration

```cpp
U u(invoke(std::forward<F>(f), *$val$));
```

is well-formed for some invented variable `u`.

::: note

There is no requirement that `U` is movable ([dcl.init.general](){.sref}).

:::

[#]{.pnum} *Returns*: If `*this` contains a value, an `optional<U>` object whose contained value is direct-non-list-initialized with `invoke(std::forward<F>(f), *$val$)`; otherwise, `optional<U>()`.

```cpp
template<class F>
  constexpr optional or_else(F&& f) const;
```

[#]{.pnum} *Constraints*: `F` models `invocable`.

[#]{.pnum} *Mandates*: `is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (has_value()) {
  return *this;
} else {
  return std::forward<F>(f)();
}
```

#### Modifiers [optional.ref.mod](){.sref} {-}

```cpp
constexpr void reset() noexcept;
```

[#]{.pnum} *Effects*: Assigns `nullptr` to `$val$`.

[#]{.pnum} *Postconditions*: `*this` does not contain a value.

#### Exposition only helper functions [optional.ref.expos](){.sref} {-}

```cpp
template<class U>
  constexpr void $convert-ref-init-val$(U&& u);
```

[#]{.pnum} *Effects*: Creates a variable `r` as if by `T& r(std::forward<U>(u));` and then initializes `$val$` with `addressof(r)`.

### No-value state indicator [optional.nullopt](){.sref} {-}

```cpp
struct nullopt_t{@\seebelow@};
inline constexpr nullopt_t nullopt(@\unspec@);
```

[#]{.pnum} The struct `nullopt_t` is an empty class type used as a unique type to indicate the state of not containing a value for `optional` objects. In particular, `optional<T>` has a constructor with `nullopt_t` as a single argument; this indicates that an optional object not containing a value shall be constructed.

[#]{.pnum} Type `nullopt_t` does not have a default constructor or an initializer-list constructor, and is not an aggregate. `nullopt_t` models `copyable` and `three_way_comparable<strong_ordering>`.

### Relational operators [optional.relops](){.sref} {-}

```cpp
template<class T, class U>
  constexpr bool operator==(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: The expression `*x == *y` is well-formed and its result is convertible to `bool`.

::: note

`T` need not be *Cpp17EqualityComparable*.

:::

[#]{.pnum} *Returns*: If `x.has_value() != y.has_value()`, `false`; otherwise if `x.has_value() == false`, `true`; otherwise `*x == *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x == *y` is a core constant expression are constexpr functions.

```cpp
template<class T, class U>
  constexpr bool operator!=(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: The expression `*x != *y` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Returns*: If `x.has_value() != y.has_value()`, `true`; otherwise, if `x.has_value() == false`, `false`; otherwise `*x != *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x != *y` is a core constant expression are constexpr functions.

```cpp
template<class T, class U>
  constexpr bool operator<(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: `*x < *y` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Returns*: If `!y`, `false`; otherwise, if `!x`, `true`; otherwise `*x < *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x < *y` is a core constant expression are constexpr functions.

```cpp
template<class T, class U>
  constexpr bool operator>(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: The expression `*x > *y` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Returns*: If `!x`, `false`; otherwise, if `!y`, `true`; otherwise `*x > *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x > *y` is a core constant expression are constexpr functions.

```cpp
template<class T, class U>
  constexpr bool operator<=(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: The expression `*x <= *y` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Returns*: If `!x`, `true`; otherwise, if `!y`, `false`; otherwise `*x <= *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x <= *y` is a core constant expression are constexpr functions.

```cpp
template<class T, class U>
  constexpr bool operator>=(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Constraints*: The expression `*x >= *y` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Returns*: If `!y`, `true`; otherwise, if `!x`, `false`; otherwise `*x >= *y`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x >= *y` is a core constant expression are constexpr functions.

```cpp
template<class T, three_way_comparable_with<T> U>
  constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const optional<U>& y);
```

[#]{.pnum} *Returns*: If `x && y`, `*x <=> *y`; otherwise `x.has_value() <=> y.has_value()`.

[#]{.pnum} *Remarks*: Specializations of this function template for which `*x <=> *y` is a core constant expression are constexpr functions.

### Comparison with `nullopt` [optional.nullops](){.sref} {-}

```cpp
template<class T>
  constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept;
```

[#]{.pnum} *Returns*: `!x`.

```cpp
template<class T>
  constexpr strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept;
```

[#]{.pnum} *Returns*: `x.has_value() <=> false`.

### Comparison with `T` [optional.comp.with.t](){.sref} {-}

```cpp
template<class T, class U>
  constexpr bool operator==(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x == v` is well-formed and its result is convertible to `bool`.

::: note

`T` need not be *Cpp17EqualityComparable*.

:::

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x == v;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator==(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v == *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v == *x;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator!=(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x != v` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x != v;
return true;
```

```cpp
template<class T, class U>
  constexpr bool operator!=(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v != *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v != *x;
return true;
```

```cpp
template<class T, class U>
  constexpr bool operator<(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x < v` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x < v;
return true;
```

```cpp
template<class T, class U>
  constexpr bool operator<(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v < *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v < *x;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator>(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x > v` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x > v;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator>(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v > *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v > *x;
return true;
```

```cpp
template<class T, class U>
  constexpr bool operator<=(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x <= v` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x <= v;
return true;
```

```cpp
template<class T, class U>
  constexpr bool operator<=(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v <= *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v <= *x;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator>=(const optional<T>& x, const U& v);
```

[#]{.pnum} *Constraints*: `U` is not a specialization of `optional`. The expression `*x >= v` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return *x >= v;
return false;
```

```cpp
template<class T, class U>
  constexpr bool operator>=(const T& v, const optional<U>& x);
```

[#]{.pnum} *Constraints*: `T` is not a specialization of `optional`. The expression `v >= *x` is well-formed and its result is convertible to `bool`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
if (x.has_value())
  return v >= *x;
return true;
```

```cpp
template<class T, class U>
  requires(!$is-derived-from-optional$<U>) && three_way_comparable_with<T, U>
  constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v);
```

[#]{.pnum} *Effects*: Equivalent to: `return x.has_value() ? *x <=> v : strong_ordering::less;`

### Specialized algorithms [optional.specalg](){.sref} {-}

```cpp
template<class T>
  constexpr void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)));
```

[#]{.pnum} *Constraints*: `is_move_constructible_v<T>` is `true` and `is_swappable_v<T>` is `true`.

[#]{.pnum} *Effects*: Calls `x.swap(y)`.

```cpp
template<int = 0, class T>
  constexpr optional<decay_t<T>> make_optional(T&& v) noexcept(is_nothrow_constructible_v<optional<decay_t<T>>, T>);
```

[#]{.pnum} *Constraints*: The call to `make_optional` does not use an explicit \grammarterm{template-argument-list} that begins with a type \grammarterm{template-argument}, and `is_constructible_v<decay_t<T>, T>` is `true`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return optional<decay_t<T>>{std::forward<T>(v)};
```

```cpp
template<class T, class... Args>
  constexpr optional<T> make_optional(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
```

[#]{.pnum} *Constraints*: `is_constructible_v<T, Args...>` is `true`.

[#]{.pnum} *Mandates*: `is_reference_v<T>` is `false`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return optional<T>{in_place, std::forward<Args>(args)...};
```

```cpp
template<class T, class U, class... Args>
  constexpr optional<T> make_optional(initializer_list<U> il, Args&&... args) noexcept(
      is_nothrow_constructible_v<T, initializer_list<U>&, Args...>);
```

[#]{.pnum} *Constraints*: `is_constructible_v<T, initializer_list<U>&, Args...>` is `true`.

[#]{.pnum} *Mandates*: `is_reference_v<U>` is `false`.

[#]{.pnum} *Effects*: Equivalent to:

```cpp
return optional<T>{in_place, il, std::forward<Args>(args)...};
```

### Hash support [optional.hash](){.sref} {-}

```cpp
template<class T> struct hash<optional<T>>;
```

[#]{.pnum} The specialization `hash<optional<T>>` is enabled ([unord.hash](){.sref}) if and only if `hash<remove_const_t<T>>` is enabled. When enabled, for an object `o` of type `optional<T>`, if `o.has_value() == true`, then `hash<optional<T>>()(o)` evaluates to the same value as `hash<remove_const_t<T>>()(*o)`; otherwise it evaluates to an unspecified value. The member functions are not guaranteed to be `noexcept`.

:::
