#include "utils.hpp"

/**
 * @brief Convert from v8 to standard string.
 * @param v v8 string
 * @return standard string
 */
std::string toString(const v8::Local<v8::String> & v) {
  v8::String::Utf8Value value(v);
  return std::string(*value);
}

/**
 * @brief Get value from object with given key name.
 * @param {v8::Local<v8::Object>} o
 * @param {const std::string &} keyName
 * @throws std::runtime_error if key could not be recovered
 * @return v8::Local<v8::Value> value
 */
v8::Local<v8::Value> GetValue(const v8::Local<v8::Object> & o, const std::string & keyName) {

  Nan::MaybeLocal<v8::Value> uncheckedValue = Nan::Get(o, Nan::New(keyName).ToLocalChecked());

  if(uncheckedValue.IsEmpty())
    throw std::runtime_error(std::string("Could not get value for key ") + keyName);
  else
    return uncheckedValue.ToLocalChecked();
}


// We have to specialize for std::string, as To returns MaybeLocal, not Maybe as above.
template<>
std::string ToNative(const v8::Local<v8::Value> & val) {

  // V8 type conversion
  Nan::MaybeLocal<v8::String> uncheckedString = Nan::To<v8::String>(val);

  if(uncheckedString.IsEmpty())
    throw std::runtime_error(std::string("Value not valid string"));
  else
    return toString(uncheckedString.ToLocalChecked());
}

namespace UnorderMap {

template<class Key, class Value>
std::unordered_map<Key, Value> decode(const v8::Local<v8::Value> & v,
                                                  const DecoderFunction<Key> & keyDecoder,
                                                  const DecoderFunction<Value> & valueDecoder) {

  /// Recover map
  if(!v->IsMap())
    throw std::runtime_error("argument must be a map");

  const v8::Map * const raw_map = v8::Map::Cast(*v);

  /// Recover array, so that we can iterate pairs

  // ::AsArray returns an array of length Size() * 2, where index N is the Nth key and index N + 1 is the Nth value.
  v8::Local<v8::Array> array_encoded_map = raw_map->AsArray();

  std::unordered_map<Key, Value> result;
  for(uint32_t i = 0;i < array_encoded_map->Length(); i += 2) {

    // Decode key
    Key key = keyDecoder(Nan::Get(array_encoded_map, i).ToLocalChecked());

    // Check that key is not already present in map
    if(result.count(key))
      throw std::runtime_error("duplicate keys in map");

    // Decode value
    Value value = valueDecoder(Nan::Get(array_encoded_map, i + 1).ToLocalChecked());

    // Insert pair in mapping
    result.insert(std::make_pair(key, value));
  }

  return result;
}

}
