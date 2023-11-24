#pragma once

#include <cstdint>
#include <fstream>
#include <string_view>
#include <vector>
#include <memory>
#include <functional>
#include <bit>
#include <nds/arm9/sassert.h>

namespace nbt {
  enum Tag : uint8_t {
    END,
    BYTE,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    BYTE_ARRAY,
    STRING,
    LIST,
    COMPOUND,
    INT_ARRAY,
    LONG_ARRAY,
  };

  static const uint8_t TOTAL_TAGS = 13;

  // flipping endianness for save viewers
  template<typename T>
  static inline T reverse_bytes(T n) {
    const size_t SIZE = sizeof(T);
    typedef std::array<uint8_t, SIZE> Bytes;

    Bytes bytes = *(Bytes*)&n;
    Bytes reversedBytes;

    for (size_t i = 0; i < SIZE; i++) {
      uint8_t byte = bytes[i];
      reversedBytes[SIZE - i - 1] = byte;
    }

    return *(T*)&reversedBytes;
  }

  // writing
  void begin_named_compound(std::ostream& s, std::string_view name);
  void close_compound(std::ostream& s);
  void begin_named_list(std::ostream& s, std::string_view name, Tag tag, int len);
  void write_named_string(std::ostream& s, std::string_view name, std::string_view value);
  void write_untagged_string(std::ostream& s, std::string_view value);
  void write_named_float(std::ostream& s, std::string_view name, float value);
  void write_named_int(std::ostream& s, std::string_view name, int value);
  void write_named_byte(std::ostream& s, std::string_view name, uint8_t value);
  void write_named_byte_array(std::ostream& s, std::string_view name, const char* values, int len);

  // reading
  Tag read_tag(std::istream& s);
  void skip_tag_payload(std::istream& s, Tag tag);
  std::string read_tagged_string(std::istream& s, Tag tag);
  void read_tagged_list(std::istream& s, Tag tag, Tag expectedElementTag, std::function<void(int)> elementCallback);
  void read_tagged_compound(std::istream& s, Tag tag, std::function<void(Tag, std::string)> callback);

  template<typename T>
  T read_raw_number(std::istream& s) {
    T value;
    s.read((char*)&value, sizeof(T));
    return nbt::reverse_bytes(value);
  }

  template<typename T>
  T read_tagged_number(std::istream& s, Tag tag) {
    switch (tag) {
    case Tag::BYTE:
      return nbt::read_raw_number<int8_t>(s);
    case Tag::SHORT:
      return nbt::read_raw_number<int16_t>(s);
    case Tag::INT:
      return nbt::read_raw_number<int32_t>(s);
    case Tag::LONG:
      return nbt::read_raw_number<int64_t>(s);
    case Tag::FLOAT:
      return nbt::read_raw_number<float>(s);
    case Tag::DOUBLE:
      return nbt::read_raw_number<double>(s);
    default:
      nbt::skip_tag_payload(s, tag);
      return 0;
    }
  }

  template<typename T>
  std::vector<T> read_byte_array(std::istream& s) {
    static_assert(sizeof(T) == 1);

    size_t len = nbt::read_raw_number<int32_t>(s);
    std::vector<T> value;
    value.resize(len);

    s.read((char*)&value[0], len);
    return value;
  }

  class Serializable {
  public:
    virtual std::string_view serializedClassName() { return "Unknown"; }

    /// Used to serialize properties into a nbt::Tag::COMPOUND
    /// The compound will begin and close outside of this function.
    virtual void serializeData(std::ostream& s) {}

    // Note for the future: It may make sense to preprocess the property for deserializeDataProperty to avoid the need for nbt::skip_tag_payload()

    /// Deserialize an individual property.
    /// The property must be read or skipped with nbt::skip_tag_payload() to allow the remainder of the stream to properly process.
    virtual void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) { nbt::skip_tag_payload(s, tag); }

    /// Generates a compound payload, excluding closing tag
    void serialize(std::ostream& s);

    /// Deserializes a compound payload, including closing tag
    template<typename T>
    static std::unique_ptr<T> deserialize(std::istream& s, nbt::Tag tag, std::unordered_map<std::string_view, std::function<std::unique_ptr<T>()>> map) {
      std::string className;
      std::unique_ptr<T> out;

      nbt::read_tagged_compound(s, tag, [&s, &map, &className, &out](nbt::Tag tag, std::string name) {
        if (name == "Class") {
          className = nbt::read_tagged_string(s, tag);
        } else if (name == "Data") {
          sassert(!name.empty(), "Expecting \"Class\" before \"Data\"");

          auto search = map.find(std::string_view(className));

          if (search != map.end() && tag == nbt::COMPOUND) {
            auto construct = search->second;
            out = construct();
            out->deserializeData(s, tag);
          } else {
            // sassert(className == "Unknown", "Failed to match Class: \"%s\"", className.c_str());
            nbt::skip_tag_payload(s, tag);
          }
        } else {
          nbt::skip_tag_payload(s, tag);
        }
      });

      return out;
    }

  private:
    void deserializeData(std::istream& s, nbt::Tag tag);
  };
}
