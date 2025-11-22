#include <iostream>
#include <cstddef>
#include <fstream>
#include <stdexcept>
namespace vasyakin
{
  void outputMatrix(const int* a, size_t rows, size_t cols, std::ofstream& output);
  int countSaddlePoints(const int* a, size_t rows, size_t cols);
  int* createMatrix(size_t rows, size_t cols);
  void transformSpiral(int* a, size_t rows, size_t cols);
  void readMatrix(int* a, size_t rows, size_t cols, std::istream& input);
}
void vasyakin::outputMatrix(const int* a, size_t rows, size_t cols, std::ofstream& output)
{
  output << rows << ' ' << cols << '\n';
  if (rows != 0 && cols != 0)
  {
    for (size_t i = 0; i < rows; ++i)
    {
      output << a[i * cols + 0];
      for (size_t j = 1; j < cols; ++j)
      {
        output << ' ' << a[i * cols + j];
      }
      output << '\n';
    }
  }
}
int vasyakin::countSaddlePoints(const int* a, size_t rows, size_t cols)
{
  int count = 0;
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < cols; ++j)
    {
      int current = a[i * cols + j];
      bool is_min_in_row = true;
      bool is_max_in_col = true;
      for (size_t f = 0; f < cols; ++f)
      {
        if (a[i * cols + f] < current)
        {
          is_min_in_row = false;
          break;
        }
      }
      for (size_t f = 0; f < rows; ++f)
      {
        if (a[f * cols + j] > current)
        {
          is_max_in_col = false;
          break;
        }
      }
      if (is_min_in_row && is_max_in_col)
      {
        count++;
      }
    }
  }
  return count;
}
int* vasyakin::createMatrix(size_t rows, size_t cols)
{
  int* a = nullptr;
  a = new int [rows * cols];
  return a;
}
void vasyakin::transformSpiral(int* a, size_t rows, size_t cols)
{
  size_t circle = std::min(rows, cols) / 2 + std::min(rows, cols) % 2;
  size_t subtrahend = 1;
  for (size_t k = 0; k < circle; ++k)
  {
    for (size_t i = k; i < cols - k; ++i)
    {
      a[k * cols + i] -= subtrahend;
      subtrahend++;
    }
    for (size_t i = k + 1; i < rows - k; ++i)
    {
      a[i * cols + (cols - 1 - k)] -= subtrahend;
      subtrahend++;
    }
    if (k < rows - k - 1)
    {
      for (size_t i = cols - k - 2; i + 1 > k; --i)
      {
        a[(rows - 1 - k) * cols + i] -= subtrahend;
        subtrahend++;
      }
    }
    if (k < cols - k - 1)
    {
      for (size_t i = rows - k - 2; i > k; --i)
      {
        a[i * cols + k] -= subtrahend;
        subtrahend++;
      }
    }
  }
}
void vasyakin::readMatrix(int* a, size_t rows, size_t cols, std::istream& input)
{
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < cols; ++j)
    {
      int temp = 0;
      if (!(input >> temp))
      {
        if (input.eof())
        {
          throw std::runtime_error("Not enough elements for matrix");
        }
        if (input.fail())
        {
          throw std::runtime_error("Unexpected input");
        }
      }
      a[i * cols + j] = temp;
    }
  }
}
int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << '\n';
    return 1;
  }
  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0')
  {
    std::cerr << "First parameter must be 1 or 2" << "\n";
    return 1;
  }
  int num = argv[1][0] - '0';
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);
  if (!input)
  {
    std::cerr << "file not open" << "\n";
    return 1;
  }
  if (!output)
  {
    std::cerr << "cannot create output file" << "\n";
    return 1;
  }
  try
  {
    size_t rows = 0, cols = 0;
    if (!(input >> rows >> cols))
    {
      std::cerr << "cannot read matrix dimensions" << "\n";
      return 2;
    }
    if (rows == 0 && cols == 0)
    {
      output << "0 0" << "\n";
      return 0;
    }
    if (num == 1)
    {
      if (rows * cols > 10000)
      {
        std::cerr << "too much" << "\n";
        return 2;
      }
      int matrix[10000];
      try
      {
        vasyakin::readMatrix(matrix, rows, cols, input);
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        return 2;
      }
      int result = vasyakin::countSaddlePoints(matrix, rows, cols);
      output << result << '\n';
      int matrix_copy[10000];
      for (size_t i = 0; i < rows * cols; ++i)
      {
        matrix_copy[i] = matrix[i];
      }
      vasyakin::transformSpiral(matrix_copy, rows, cols);
      vasyakin::outputMatrix(matrix_copy, rows, cols, output);
    }
    else
    {
      int* matrix = vasyakin::createMatrix(rows, cols);
      try
      {
        vasyakin::readMatrix(matrix, rows, cols, input);
      }
      catch (const std::exception& e)
      {
        delete[] matrix;
        std::cerr << e.what() << '\n';
        return 2;
      }
      int result = vasyakin::countSaddlePoints(matrix, rows, cols);
      output << result << '\n';
      int* matrix_copy = vasyakin::createMatrix(rows, cols);
      for (size_t i = 0; i < rows * cols; ++i)
      {
        matrix_copy[i] = matrix[i];
      }
      vasyakin::transformSpiral(matrix_copy, rows, cols);
      vasyakin::outputMatrix(matrix_copy, rows, cols, output);
      delete[] matrix;
      delete[] matrix_copy;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }
  return 0;
}
