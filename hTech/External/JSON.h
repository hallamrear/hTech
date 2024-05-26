#pragma once

//rapidjson
#include "External/rapidjson/rapidjson.h"
#include "External/rapidjson/stringbuffer.h"
#include "External/rapidjson/prettywriter.h"
#include "External/rapidjson/document.h"

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> Serializer;
typedef rapidjson::Document Deserializer;
typedef rapidjson::Value SerializedValue;