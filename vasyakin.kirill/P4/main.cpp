#include <iostream>
#include <cstddef>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <limits>
namespace vasyakin
{
  size_t stringLength(const char* str);
  void cutLetters(const char* str, char* str1);
  void excludeCharsFromSecond(const char* str1, const char* str2, char* result);
  size_t extend(char*& a, size_t k, size_t k1);
  size_t getline(std::istream& input, size_t& k, char*& str);
  const size_t MAX = std::numeric_limits< size_t >::max();
}
size_t vasyakin::stringLength(const char* str)
{
  size_t count = 0;
  while(str[count] != '\0')
  {
    count++;
  }
  return count + 1;
}
size_t vasyakin::extend(char*& a, size_t k, size_t k1)
{
  char* new_arr = reinterpret_cast< char* >(malloc(sizeof(char) * k1));
  if (new_arr == nullptr)
  {
    return 1;
  }
  for (size_t i = 0; i < k; ++i)
  {
    new_arr[i] = a[i];
  }
  free(a);
  a = new_arr;
  return 0;
}
size_t vasyakin::getline(std::istream& input, size_t& k, char*& str)
{
  str = reinterpret_cast< char* >(malloc(sizeof(char)));
  if (str == nullptr)
  {
    return 1;
  }
  str[0] = '\0';
  k = 0;
  bool is_skipws = input.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    input >> std::noskipws;
  }
  char i = 0;
  while (input >> i && i != '\n')
  {
    if (k + 1 > vasyakin::MAX - 1)
    {
      return 2;
    }
    size_t check = extend(str, k, k + 2);
    if (check == 1)
    {
      return 1;
    }
    str[k] = i;
    str[k + 1] = '\0';
    ++k;
  }
  if (is_skipws)
  {
    input >> std::skipws;
  }
  return 0;
}
void vasyakin::cutLetters(const char* str, char* str1)
{
  size_t j = 0;
  for (size_t i = 0; str[i] != '\0'; ++i)
  {
    if (!std::isalpha(static_cast< unsigned char >(str[i])))
    {
      str1[j] = str[i];
      j++;
    }
  }
  str1[j] = '\0';
}
void vasyakin::excludeCharsFromSecond(const char* str1, const char* str2, char* result)
{
  size_t j = 0;
  for (size_t i = 0; str1[i] != '\0'; ++i)
  {
    bool flag = false;
    for (size_t k = 0; str2[k] != '\0'; ++k)
    {
      if (str1[i] == str2[k])
      {
        flag = true;
        break;
      }
    }
    if (!flag)
    {
      result[j] = str1[i];
      j++;
    }
  }
  result[j] = '\0';
}
int main()
{
  size_t k = 0;
  char * str = nullptr;
  size_t check = vasyakin::getline(std::cin, k, str);
  if (check == 1)
  {
    std::cerr << "Memory allocation failed" << '\n';
    return 1;
  }
  else if (check == 2)
  {
    std::cerr << "Overflow error" << '\n';
    free(str);
    return 2;
  }
  if (std::cin.fail() && !std::cin.eof())
  {
    std::cerr << "Input stream in fail state" << '\n';
    free(str);
    return 1;
  }
  if (str[0] == '\0')
  {
    std::cerr << "Empty string" << '\n';
    free(str);
    return 1;
  }
  size_t str_len = vasyakin::stringLength(str);
  char* result1 = reinterpret_cast< char* >(malloc(sizeof(char) * str_len));
  char* result2 = reinterpret_cast< char* >(malloc(sizeof(char) * str_len));
  if (!result1 || !result2)
  {
    std::cerr << "Memory allocation failed" << '\n';
    free(str);
    if (result1)
    {
      free(result1);
    }
    if (result2)
    {
      free(result2);
    }
    return 1;
  }
  const char* str1 = "abc";
  vasyakin::cutLetters(str, result1);
  vasyakin::excludeCharsFromSecond(str, str1, result2);
  std::cout << result1 << '\n';
  std::cout << result2 << '\n';
  free(str);
  free(result1);
  free(result2);
  return 0;
}
