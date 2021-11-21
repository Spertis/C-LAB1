//Copyright 2021 Spertis

#define TEST_CPP_
#ifdef TEST_CPP_


#include <gtest/gtest.h>
#include "header.hpp"
#include <Student.hpp>


#ifndef _JSON_DIR
#define JSON_DIR "nobody"
#else
#define JSON_DIR _JSON_DIR
#endif


TEST(printTest, StudentItem)
{
  std::stringstream ss;
  Student s("test", (std::string)"test", (size_t)3, std::vector<std::string>());
  print(s, ss);
  ASSERT_EQ("| test                | test      | 3         | 0         |\n"
      "|---------------------|-----------|-----------|-----------|\n",  ss.str());
}
TEST(printTest, StudentsArray) {
    std::stringstream ss;
    std::vector<Student> students;
    students.emplace_back("test", (std::string)"test",
                          (size_t)3, std::vector<std::string>());
    students.emplace_back("test", (size_t)4,
                          (double)3.33, std::vector<std::string>());
    print(students, ss);
    ASSERT_EQ("| name                | group     | avg       | debt      |\n"
        "|---------------------|-----------|-----------|-----------|\n"
        "| test                | test      | 3         | 0         |\n"
        "|---------------------|-----------|-----------|-----------|\n"
        "| test                | 4         | 3.33      | 0         |\n"
        "|---------------------|-----------|-----------|-----------|\n",  ss.str());
}
TEST(printTest, json)
{
  std::string jsonData = "{\n"
      "  \"items\": [\n"
      "    {\n"
      "      \"name\": \"Ivanov Ivan\",\n"
      "      \"group\": \"U8+2-32\",\n"
      "      \"avg\": \"4.95\",\n"
      "      \"debt\": null\n"
      "    },\n"
      "    {\n"
      "      \"name\": \"Machine Robot\",\n"
      "      \"group\": 15,\n"
      "      \"avg\": 6.66,\n"
      "      \"debt\": \"C\"\n"
      "    }\n"
      "  ],\n"
      "  \"_meta\": {\n"
      "    \"count\": 2\n"
      "  }\n"
      "}";
  nlohmann::json _json = json::parse(jsonData);
  std::vector<Student> students_parsed = parsingJson(_json);
  std::stringstream ss;
  print(students_parsed, ss);
  ASSERT_EQ("| name                | group     | avg       | debt      |\n"
      "|---------------------|-----------|-----------|-----------|\n"
      "| Ivanov Ivan         | U8+2-32   | 4.95      | null      |\n"
      "|---------------------|-----------|-----------|-----------|\n"
      "| Machine Robot       | 15        | 6.66      | C         |\n"
      "|---------------------|-----------|-----------|-----------|\n", ss.str());
}

TEST(parseTest, jsonParse)
{
  std::string jsonData = "{\n"
      "  \"items\": [\n"
      "    {\n"
      "      \"name\": \"Ivanov Ivan\",\n"
      "      \"group\": \"IU8-32\",\n"
      "      \"avg\": \"5.25\",\n"
      "      \"debt\": null\n"
      "    },\n"
      "    {\n"
      "      \"name\": \"Machine Robot\",\n"
      "      \"group\": 32,\n"
      "      \"avg\": 6.66,\n"
      "      \"debt\": \"C\"\n"
      "    }\n"
      "  ],\n"
      "  \"_meta\": {\n"
      "    \"count\": 2\n"
      "  }\n"
      "}";
    nlohmann::json _json = json::parse(jsonData);
    std::vector<Student> students_parsed = parsingJson(_json);
    std::vector<Student> student_inited = {
        Student("Ivanov Ivan", (std::string)"IU8-32",
                (std::string)"5.25", nullptr),
        Student("Machine Robot", (size_t)32,
                (double)6.66, (std::string)"C")
    };
    ASSERT_EQ(student_inited, students_parsed);
}
TEST(parseTest, fromFile)
{
  std::string jsonData = "{\n"
      "  \"items\": [\n"
      "    {\n"
      "      \"name\": \"Ivanov Ivan\",\n"
      "      \"group\": \"2\",\n"
      "      \"avg\": \"6.66\",\n"
      "      \"debt\":  [\n"
      "        \"Java\",\n"
      "        \"Python\",\n"
      "        \"LoL\"\n"
      "      ]\n"
      "    },\n"
      "    {\n"
      "      \"name\": \"Terr Terr\",\n"
      "      \"group\": 32,\n"
      "      \"avg\": 3.75,\n"
      "      \"debt\": \"LoL\"\n"
      "    }\n"
      "  ],\n"
      "  \"_meta\": {\n"
      "    \"count\": 2\n"
      "  }\n"
      "}";
  nlohmann::json json1 = json::parse(jsonData);
  std::string path = JSON_DIR;
  path+="/test1.json";
  char* argv[] ={(char*)"", (char*)(path.c_str())};
  nlohmann::json json2 = takeJson(2, argv);
  ASSERT_EQ(json1, json2) << "Test passed!";
}

TEST(errorCheck, lessArgsTest)
{
  try {
    std::string path = JSON_DIR;
    path+="/test1.json";
    char* argv[] ={(char*)"", (char*)(path.c_str())};
    nlohmann::json json2 = takeJson(1, argv);
    FAIL() << "Expected: The file path was not passed";
  }
  catch(std::runtime_error const & err) {
    EXPECT_EQ(err.what(),std::string("The file path was not passed"));
  }
  catch(...) {
    FAIL() << "Expected The file path was not passed";
  }
}

TEST(errorCheck, _metaCheck)
{
  try {
    std::string path = JSON_DIR;
    path+="/test2.json";
    char* argv[] ={(char*)"", (char*)(path.c_str())};
    nlohmann::json json2 = takeJson(2, argv);
    FAIL() << "Expected: _meta value does not match the array size";
  }
  catch(std::runtime_error const & err) {
    EXPECT_EQ(err.what(),std::string("_meta value does not match the array size"));
  }
  catch(...) {
    FAIL() << "Expected: _meta value does not match the array size";
  }
}
#endif // TEST_CPP_