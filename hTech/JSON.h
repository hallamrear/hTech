#pragma once

//rapidjson
#include <rapidjson.h>
#include <stringbuffer.h>
#include <prettywriter.h>
#include <document.h>

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> Serializer;
typedef rapidjson::Document Deserializer;
typedef rapidjson::Value SerializedValue;