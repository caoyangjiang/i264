/**
 * @file bits.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_BITS_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_BITS_H_

#include <array>
#include <cstring>
#include <exception>
#include <limits>
#include <type_traits>
#include <vector>

namespace i264 {
/**
 * @brief A bit container similar to std::vector
 * Author
 *  - Caoyang Jiang
 *
 * Require
 *  - c++17 standard.
 *
 * Optimization
 *  - Buffer initialize to zero. So push zero == do nothing
 *  - Pre-construct individual bit mask for writing of ones.
 *  - Pre-construct mask for cleaning unused bits
 *
 * Features:
 *  - No space waste (No bits == zero buffer size)
 *  - Forward iterator (facilitate insert, random access, segmentation)
 *  - Fast read  (300~400 Mb/s)
 *  - Fast write (3000~4000 Mb/s)
 *  - Configurable bit ordering (MSB/LSB first)
 *  - Configurable buffer type (any unsigned type)
 *
 * Caveats:
 * It is not possible to dereference the iterator to get the bit value
 * since this is sub-byte access. The dereferenced iterator returns "BitHolder"
 * type and user can use "Value()" method to read the value. "BitHolder" is
 * assignable and it can be used to change the bit value at the iterator
 * location.
 */
template <class BUFFER_ELEMENT_TYPE = unsigned char, bool MSB_TO_LSB = true,
          class = typename std::enable_if<
              std::is_unsigned<BUFFER_ELEMENT_TYPE>::value>::type>
class Bit {
 public:
  typedef BUFFER_ELEMENT_TYPE value_type;
  typedef BUFFER_ELEMENT_TYPE& reference;
  typedef const BUFFER_ELEMENT_TYPE& const_reference;
  typedef size_t size_type;
  typedef std::vector<value_type> container_type;

  static const value_type ONE = value_type(1);
  static const value_type ZERO = value_type(0);
  static const value_type VALUE_MAX = std::numeric_limits<value_type>::max();
  static const size_t VALUE_TYPE_BTYE_SIZE = sizeof(value_type);
  static const size_t VALUE_TYPE_BIT_SIZE = 8 * sizeof(value_type);

 public:
  struct BitHolder {
    size_type bit_position_ = 0;
    container_type* container_ = nullptr;

    BitHolder& CopyValueFrom(const BitHolder& value) {
      bit_position_ = value.bit_position_;
      container_ = value.container_;
      return *this;
    }

    BitHolder& AssignValue(value_type value) {
      size_type byte_position = bit_position_ / VALUE_TYPE_BIT_SIZE;
      size_type bit_position = bit_position_ % VALUE_TYPE_BIT_SIZE;

      if (value == ONE) {
        (*container_)[byte_position] |= BIT_PATTERNS[bit_position];
      }

      if (value == ZERO) {
        (*container_)[byte_position] &= ~BIT_PATTERNS[bit_position];
      }

      return *this;
    }

    BitHolder& operator=(value_type value) { return AssignValue(value); }

    BitHolder& operator=(const BitHolder& value) {
      return AssignValue(value.Value());
    }

    bool operator==(const BitHolder& bit) const {
      return bit_position_ == bit.bit_position_;
    }

    bool operator!=(const BitHolder& bit) const {
      return bit_position_ != bit.bit_position_;
    }

    value_type Value() const {
      size_type byte_position = bit_position_ / VALUE_TYPE_BIT_SIZE;
      size_type bit_position = bit_position_ % VALUE_TYPE_BIT_SIZE;
      value_type byte = (*container_)[byte_position];
      return (byte & BIT_PATTERNS[bit_position]) == ZERO ? ZERO : ONE;
    }
  };

 public:
  template <class A, bool B, class C>
  friend Bit<A, B, C> operator+(const Bit<A, B, C>& lhs,
                                const Bit<A, B, C>& rhs);

 private:
  static constexpr value_type CreateBitPattern(size_t i) {
    return ONE << (MSB_TO_LSB ? (VALUE_TYPE_BIT_SIZE - 1) - i : i);
  }

  template <size_t... i>
  static constexpr auto CreateBitPattern(std::index_sequence<i...>) {
    return std::array<value_type, sizeof...(i)>{{CreateBitPattern(i)...}};
  }

  static constexpr auto CreateBitPattern() {
    return CreateBitPattern(std::make_index_sequence<VALUE_TYPE_BIT_SIZE>{});
  }

  static constexpr value_type create_mask_pattern(size_t i) {
    value_type mask = ZERO;
    for (size_t d = 0; d <= i; d++) mask |= CreateBitPattern(d);
    return mask;
  }

  template <size_t... i>
  static constexpr auto CreateMaskPatternArray(std::index_sequence<i...>) {
    return std::array<value_type, sizeof...(i)>{{create_mask_pattern(i)...}};
  }

  static constexpr auto CreateMaskPatternArray() {
    return CreateMaskPatternArray(
        std::make_index_sequence<VALUE_TYPE_BIT_SIZE>{});
  }

 private:
  /**
   * @brief BIT_PATTERNS are
   * For example (BUFFER_ELEMENT_TYPE=uint8_t)
   *  For MSB to LSB,
   *    "10000000", "01000000", "00100000", "00010000",  "00001000",
   * "00000100", "00000010", "00000001"
   *
   *  For LSB to MSB,
   *    "00000001", "00000010", "00000100", "00001000",  "00010000",
   * "00100000", "01000000", "10000000"
   */
  static constexpr auto BIT_PATTERNS = CreateBitPattern();

  /**
   * @brief MASK_PATTERNS are
   *
   * For example (BUFFER_ELEMENT_TYPE=uint8_t)
   *  For MSB to LSB,
   *    "10000000", "11000000", "11100000", "11110000",  "11111000",
   * "11111100", "11111110", "11111111"
   *
   *  For LSB to MSB,
   *    "00000001", "00000011", "00000111", "00001111",  "00011111",
   * "00111111", "01111111", "11111111"
   */
  static constexpr auto MASK_PATTERNS = CreateMaskPatternArray();

  template <bool IsConst>
  class BaseIterator
      : public std::iterator<std::forward_iterator_tag, BitHolder> {
   public:
    using difference_type = typename BaseIterator::difference_type;

   public:
    friend BaseIterator<!IsConst>;

   public:
    explicit BaseIterator(BitHolder x) { p.CopyValueFrom(x); }

    BaseIterator(const BaseIterator<true>& it) { p.CopyValueFrom(it.p); }

    BaseIterator(const BaseIterator<false>& it) { p.CopyValueFrom(it.p); }

    BaseIterator<IsConst>& operator=(
        const BaseIterator<true>& it)  // prefix increment
    {
      p.CopyValueFrom(it.p);
      return *this;
    }

    BaseIterator<IsConst>& operator=(
        const BaseIterator<false>& it)  // prefix increment
    {
      p.CopyValueFrom(it.p);
      return *this;
    }

    BaseIterator<IsConst>& operator++()  // prefix increment
    {
      ++(p.bit_position_);
      return *this;
    }

    BaseIterator<IsConst> operator++(int)  // postfix increment
    {
      BaseIterator<IsConst> tmp(*this);
      operator++();
      return tmp;
    }

    BaseIterator operator+(difference_type n) const {
      return BaseIterator{BitHolder{p.bit_position_ + n, p.container_}};
    }

    BaseIterator operator-(difference_type n) const {
      return BaseIterator{BitHolder{p.bit_position_ - n, p.container_}};
    }

    template <bool T>
    bool operator==(const BaseIterator<T>& rhs) const {
      return p == rhs.p;
    }

    template <bool T>
    bool operator!=(const BaseIterator<T>& rhs) const {
      return p != rhs.p;
    }

    typename std::conditional<IsConst, const BitHolder&, BitHolder&>::type
    operator*() {
      return p;
    }

   private:
    BitHolder p;
  };

 public:
  typedef BaseIterator<false> Iterator;
  typedef BaseIterator<true> ConstIterator;

 public:
  // constructor
  Bit();

  explicit Bit(size_type nbits);
  Bit(size_type nbits, const value_type& val);
  Bit(const Bit& x);
  Bit(Bit&& x);

  template <class IL_TYPE>
  Bit(std::initializer_list<IL_TYPE> il);

  Bit(const value_type* data, size_type nbits);
  //  Bit(const value_type* data, size_type begin, size_type end);

  template <class InputIterator>
  Bit(InputIterator first, InputIterator last);

  // destructor
  ~Bit();

  // assignment operator
  Bit& operator=(const Bit& x);
  Bit& operator=(Bit&& x);

  template <class IL_TYPE>
  Bit& operator=(std::initializer_list<IL_TYPE> il);

  // iterator
  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;

  // capacity
  size_type Size() const noexcept;
  void Resize(size_type nbits);
  void Resize(size_type nbits, const value_type& val);
  void Reserve(size_type nbits);
  size_type BufferElementCount() const noexcept;
  size_type BufferElementSize() const noexcept;
  bool Empty() const noexcept;

  // element access
  value_type operator[](size_type bit_position) const;
  value_type At(size_type bit_position) const;
  value_type Front() const noexcept;
  value_type Back() const noexcept;
  const value_type* Data() const noexcept;
  value_type* Data() noexcept;
  // size_type Capacity() const noexcept;

  // modifiers
  void Push(value_type value);
  void Push(const value_type* data, size_t nbits);
  void Pop();
  void Replace(size_type bit_position, value_type value);
  void Align(value_type value);
  Iterator Erase(ConstIterator position);
  Iterator Insert(ConstIterator position, const value_type& value);
  // Iterator Insert(ConstIterator position, size_type n, const value_type&
  // value); Iterator Insert(ConstIterator position,
  // std::initializer_list<value_type> il); template <class InputIterator>
  // Iterator Insert(ConstIterator position,
  //                 InputIterator first,
  //                 InputIterator last);

  void Clear() noexcept;

 private:
  inline void PushWithResize(value_type value);
  inline void PopWithResize();
  inline value_type BitValueAt(size_type position) const;
  inline value_type BitValue(size_type position) const;
  inline size_type BitCount() const noexcept;
  inline size_type BitRemainder() noexcept;

  template <class IL_TYPE>
  inline void InitializeFrom(const std::initializer_list<IL_TYPE>& type);

 private:
  container_type buffer_;
  value_type* last_byte_holder_ = nullptr;
  size_type next_bit_position_ = VALUE_TYPE_BIT_SIZE;
};

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK> operator+(
    const Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>& lhs,
    const Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>& rhs) {
  Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK> concat = lhs;

  if (concat.next_bit_position_ ==
      Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::VALUE_TYPE_BIT_SIZE) {
    size_t buffer_count = concat.BufferElementCount();
    concat.buffer_.resize(rhs.BufferElementCount() +
                          concat.BufferElementCount());
    concat.last_byte_holder_ = &(concat.buffer_.back());
    std::memcpy(concat.buffer_.data() + buffer_count, rhs.Data(),
                rhs.BufferElementCount());
  } else {
    for (typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type i =
             0;
         i < rhs.Size(); i++) {
      concat.Push(rhs[i]);
    }
  }

  return concat;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit() {
  Clear();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(size_type n) {
  Resize(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(size_type n,
                                                     const value_type& val) {
  Resize(n, val);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
template <class InputIterator>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(InputIterator first,
                                                     InputIterator last) {
  auto it = first;
  while (it != last) {
    Push((*it).Value());
    it++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(const Bit& x) {
  *this = x;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(Bit&& x) {
  *this = std::move(x);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
template <class IL_TYPE>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(
    std::initializer_list<IL_TYPE> il) {
  InitializeFrom(il);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Bit(const value_type* data,
                                                     size_type size) {
  Resize(size);
  std::memcpy(buffer_.data(), data,
              VALUE_TYPE_BTYE_SIZE * (size + VALUE_TYPE_BIT_SIZE - 1) /
                  VALUE_TYPE_BIT_SIZE);

  // clean up tail if necessary
  if (next_bit_position_ != VALUE_TYPE_BIT_SIZE) {
    *last_byte_holder_ &= MASK_PATTERNS[next_bit_position_ - 1];
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::~Bit() {}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(const Bit& x) {
  buffer_ = x.buffer_;
  last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = x.next_bit_position_;
  return *this;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(Bit&& x) {
  buffer_ = std::move(x.buffer_);
  last_byte_holder_ = x.last_byte_holder_;
  next_bit_position_ = x.next_bit_position_;
  x.last_byte_holder_ = nullptr;
  x.next_bit_position_ = VALUE_TYPE_BIT_SIZE;
  return *this;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
template <class IL_TYPE>
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(
    std::initializer_list<IL_TYPE> il) {
  InitializeFrom(il);
  return *this;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::begin() {
  return Iterator(BitHolder{0, &buffer_});
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::end() {
  return Iterator(
      BitHolder{(buffer_.size() - 1) * VALUE_TYPE_BIT_SIZE + next_bit_position_,
                &buffer_});
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::ConstIterator
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::begin() const {
  return ConstIterator(BitHolder{0, &buffer_});
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::ConstIterator
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::end() const {
  return ConstIterator(
      BitHolder{(buffer_.size() - 1) * VALUE_TYPE_BIT_SIZE + next_bit_position_,
                &buffer_});
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::PushWithResize(
    value_type value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  if (next_bit_position_ == VALUE_TYPE_BIT_SIZE) {
    buffer_.resize(buffer_.size() + 1);
    last_byte_holder_ = &(buffer_.back());
    *(last_byte_holder_) = ZERO;
    next_bit_position_ = 0;
  }

  if (value == ONE) {
    *(last_byte_holder_) =
        *(last_byte_holder_) | BIT_PATTERNS[next_bit_position_];
  }

  next_bit_position_++;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::PopWithResize() {
  if (next_bit_position_ == 0) {
    if (buffer_.size() > 1) {
      buffer_.pop_back();
      last_byte_holder_ = &(buffer_.back());
      next_bit_position_ = VALUE_TYPE_BIT_SIZE;
    }
  }

  next_bit_position_--;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Clear() noexcept {
  buffer_.clear();
  next_bit_position_ = VALUE_TYPE_BIT_SIZE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BitValueAt(
    size_type position) const {
  if (position >=
      (buffer_.size() - 1) * VALUE_TYPE_BIT_SIZE + next_bit_position_) {
    throw std::out_of_range("bit query position is out of range.");
  }

  return BitValue(position);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BitValue(
    size_type position) const {
  size_type byte_position = position / VALUE_TYPE_BIT_SIZE;
  size_type bit_position = position % VALUE_TYPE_BIT_SIZE;
  value_type byte = buffer_.operator[](byte_position);
  return (byte & BIT_PATTERNS[bit_position]) == ZERO ? ZERO : ONE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BitCount() const noexcept {
  return (buffer_.size() - 1) * VALUE_TYPE_BIT_SIZE + next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BitRemainder() noexcept {
  return VALUE_TYPE_BIT_SIZE - next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
template <class IL_TYPE>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::InitializeFrom(
    const std::initializer_list<IL_TYPE>& il) {
  Resize(il.size());

  size_type i = 0;
  for (auto& value : il) {
    if ((static_cast<BIT_CONTAINER_TYPE>(value) != ZERO) &&
        (static_cast<BIT_CONTAINER_TYPE>(value) != ONE)) {
      throw std::invalid_argument("input argument is not one or zero.");
    }

    Replace(i, static_cast<BIT_CONTAINER_TYPE>(value));
    i++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Size() const noexcept {
  return BitCount();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Reserve(size_type n) {
  buffer_.reserve((n + VALUE_TYPE_BIT_SIZE - 1) / VALUE_TYPE_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Resize(size_type n) {
  buffer_.resize((n + VALUE_TYPE_BIT_SIZE - 1) / VALUE_TYPE_BIT_SIZE);
  if (n != 0) last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = (n % VALUE_TYPE_BIT_SIZE) == 0
                           ? VALUE_TYPE_BIT_SIZE
                           : (n % VALUE_TYPE_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Resize(
    size_type n, const value_type& value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  const value_type element_value = value == ONE ? VALUE_MAX : ZERO;
  buffer_.resize((n + VALUE_TYPE_BIT_SIZE - 1) / VALUE_TYPE_BIT_SIZE,
                 element_value);
  if (n != 0) last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = (n % VALUE_TYPE_BIT_SIZE) == 0
                           ? VALUE_TYPE_BIT_SIZE
                           : (n % VALUE_TYPE_BIT_SIZE);

  // keep unused bits zero
  if (n != 0) *last_byte_holder_ &= MASK_PATTERNS[next_bit_position_ - 1];
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BufferElementCount() const
    noexcept {
  return buffer_.size();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::BufferElementSize() const
    noexcept {
  return VALUE_TYPE_BTYE_SIZE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bool Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Empty() const noexcept {
  return BitCount() == 0;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
    Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator[](
        size_type n) const {
  return BitValue(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::At(size_type n) const {
  return BitValueAt(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Front() const noexcept {
  return BitValue(0);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Back() const noexcept {
  return BitValue(BitCount() - 1);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
const typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type*
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Data() const noexcept {
  return buffer_.data();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type*
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Data() noexcept {
  return buffer_.data();
}
template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Push(value_type value) {
  PushWithResize(value);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Push(
    const BIT_CONTAINER_TYPE* data, size_type nbits) {
  Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK> tmp(data, nbits);
  *this = *this + tmp;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Pop() {
  PopWithResize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Replace(
    size_type position, value_type value) {
  size_type byte_position = position / VALUE_TYPE_BIT_SIZE;
  size_type bit_position = position % VALUE_TYPE_BIT_SIZE;

  if (value == ONE) {
    buffer_.at(byte_position) |= BIT_PATTERNS[bit_position];
  }

  if (value == ZERO) {
    buffer_.at(byte_position) &= ~BIT_PATTERNS[bit_position];
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Align(value_type value) {
  int b = 0;
  while (next_bit_position_ != VALUE_TYPE_BIT_SIZE) {
    Push((value & BIT_PATTERNS[b]) == ZERO ? ZERO : ONE);
    b++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator
Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Erase(ConstIterator position) {
  if (position != end()) {
    typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator result =
        position;
    typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator first =
        position;
    first++;

    Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type moved_bits = 0;
    while (first != end()) {
      *result = std::move(*first);
      ++result;
      ++first;
      ++moved_bits;
    }

    Resize(Size() - 1);
  }

  return position;
}

// template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
// typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Iterator
// Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::Insert(
//     ConstIterator position,
//     const typename Bit<BIT_CONTAINER_TYPE, MSB_TO_LSB,
//     TYPE_CHECK>::value_type&
//         value)
// {
//   std::vector<uint8_t>::iterator s, t, k;
//   k = s - t;
// }
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_BITS_H_
