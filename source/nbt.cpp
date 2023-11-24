#include "nbt.h"

// writing

static inline void write_byte(std::ostream& s, int8_t n) {
  s << n;
}

static inline void write_short(std::ostream& s, int16_t n) {
  n = nbt::reverse_bytes(n);
  s.write((const char*)&n, 2);
}

static inline void write_int(std::ostream& s, int32_t n) {
  n = nbt::reverse_bytes(n);
  s.write((const char*)&n, 4);
}

void nbt::begin_named_compound(std::ostream& s, std::string_view name) {
  write_byte(s, nbt::Tag::COMPOUND);
  write_untagged_string(s, name);
}

void nbt::close_compound(std::ostream& s) {
  write_byte(s, nbt::Tag::END);
}

void nbt::begin_named_list(std::ostream& s, std::string_view name, Tag tag, int len) {
  write_byte(s, nbt::Tag::LIST);
  write_untagged_string(s, name);
  write_byte(s, tag);
  write_int(s, len);
}

void nbt::write_named_string(std::ostream& s, std::string_view name, std::string_view value)
{
  write_byte(s, nbt::Tag::STRING);
  write_untagged_string(s, name);
  write_untagged_string(s, value);
}

void nbt::write_untagged_string(std::ostream& s, std::string_view value)
{
  write_short(s, value.size());
  s << value;
}

void nbt::write_named_float(std::ostream& s, std::string_view name, float value)
{
  write_byte(s, nbt::Tag::FLOAT);
  write_untagged_string(s, name);
  write_int(s, *(int32_t*)&value);
}

void nbt::write_named_int(std::ostream& s, std::string_view name, int value)
{
  write_byte(s, nbt::Tag::INT);
  write_untagged_string(s, name);
  write_int(s, value);
}

void nbt::write_named_byte(std::ostream& s, std::string_view name, uint8_t value)
{
  write_byte(s, nbt::Tag::BYTE);
  write_untagged_string(s, name);
  write_byte(s, value);
}

void nbt::write_named_byte_array(std::ostream& s, std::string_view name, const char* values, int len)
{
  write_byte(s, nbt::Tag::BYTE_ARRAY);
  write_untagged_string(s, name);
  write_int(s, len);
  s.write(values, len);
}


// reading

nbt::Tag nbt::read_tag(std::istream& s)
{
  uint8_t byte;
  s.read((char*)&byte, 1);
  return (nbt::Tag)byte;
}

std::string nbt::read_tagged_string(std::istream& s, Tag tag)
{
  if (tag != nbt::Tag::STRING) {
    skip_tag_payload(s, tag);
    return "";
  }

  size_t len = nbt::read_tagged_number<size_t>(s, nbt::Tag::SHORT);

  std::string value;
  value.resize(len);

  s.read((char*)&value[0], len);

  return value;
}


void nbt::read_tagged_list(std::istream& s, Tag tag, Tag expectedElementTag, std::function<void(int)> elementCallback)
{
  if (tag != nbt::Tag::LIST) {
    nbt::skip_tag_payload(s, tag);
    return;
  }

  auto elementTag = nbt::read_tag(s);
  int len = nbt::read_raw_number<int32_t>(s);

  if (elementTag != expectedElementTag) {
    for (int i = 0; i < len; i++) {
      nbt::skip_tag_payload(s, elementTag);
    }
  } else {
    for (int i = 0; i < len; i++) {
      elementCallback(i);
    }
  }
}

void nbt::read_tagged_compound(std::istream& s, nbt::Tag tag, std::function<void(nbt::Tag, std::string)> callback)
{
  if (tag != nbt::Tag::COMPOUND) {
    nbt::skip_tag_payload(s, tag);
    return;
  }

  nbt::Tag propertyTag;

  while ((propertyTag = nbt::read_tag(s)) != nbt::Tag::END) {
    auto name = nbt::read_tagged_string(s, nbt::Tag::STRING);

    callback(propertyTag, name);
  }
}

void nbt::skip_tag_payload(std::istream& s, nbt::Tag tag) {
  switch (tag) {
  case END:
    break;
  case BYTE:
    s.ignore(1);
    break;
  case SHORT:
    s.ignore(2);
    break;
  case INT:
  case FLOAT:
    s.ignore(4);
    break;
  case LONG:
  case DOUBLE:
    s.ignore(8);
    break;
  case BYTE_ARRAY:
    s.ignore(nbt::read_raw_number<int32_t>(s));
    break;
  case STRING:
    s.ignore(nbt::read_tagged_number<size_t>(s, nbt::Tag::SHORT));
    break;
  case LIST: {
    nbt::Tag tag = nbt::read_tag(s);
    int len = nbt::read_raw_number<int32_t>(s);

    for (int i = 0; i < len; i++) {
      nbt::skip_tag_payload(s, tag);
    }
    break;
  }
  case INT_ARRAY:
    s.ignore(nbt::read_raw_number<int32_t>(s) * 4);
    break;
  case LONG_ARRAY:
    s.ignore(nbt::read_raw_number<int32_t>(s) * 8);
    break;
  case COMPOUND: {
    nbt::Tag innerTag;

    while ((innerTag = nbt::read_tag(s)) != nbt::Tag::END) {
      nbt::skip_tag_payload(s, nbt::Tag::STRING);
      nbt::skip_tag_payload(s, innerTag);
    }
    break;
  }
  default:
    sassert(false, "Unknown NBT Tag: %d", tag);
  }
}

void nbt::Serializable::serialize(std::ostream& s) {
  nbt::write_named_string(s, "Class", serializedClassName());
  nbt::begin_named_compound(s, "Data");
  serializeData(s);
  nbt::close_compound(s);
}

void nbt::Serializable::deserializeData(std::istream& s, nbt::Tag tag) {
  nbt::read_tagged_compound(s, tag, [this, &s](nbt::Tag tag, std::string name) {
    deserializeDataProperty(s, tag, name);
  });
}
