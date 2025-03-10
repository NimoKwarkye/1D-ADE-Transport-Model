#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string>
#include <execution>
#include <sstream>
#include <fstream>
#include <iomanip>

//#include "MatArray.h"
namespace nims_n
{
	template<typename T>
	class ReadCSV
	{
		void getFileInfo(const std::string& csvFile, char delimit, bool hasHeader, std::vector<int>& skipRows, std::vector<int>& skipCols, std::int64_t indexCol)
		{
			std::ifstream fileStream;
			std::string line;
			std::int64_t curPos = 0;
			bool rdh{ false };
			fileStream.open(csvFile);
			if(fileStream.is_open())
			{
				while (std::getline(fileStream, line))
				{
					if (line.empty())
						break;
					if (std::find(skipRows.begin(), skipRows.end(), curPos) == skipRows.end())
					{

						if (!rdh)
						{
							readHeader(line, delimit, indexCol, skipCols, hasHeader);
							rdh = true;
						}



					}
					curPos++;

				}
				_nrows = hasHeader ? curPos - skipRows.size() - 1 : curPos - skipRows.size();


				_data = std::vector<T>(_nrows * _ncols, T(0));
				fileStream.close();
			}
			else
			{
				throw std::runtime_error("unable to open file: " + csvFile);
			}
			
		}

		void readHeader(std::string& line, char delimit, int indexCol, std::vector<int>& skipCols, bool hasHeader)
		{
			std::string col = "";
			std::stringstream ls(line);
			_columns.clear();

			size_t idx{ 0 };
			if(hasHeader)
			{
				while (std::getline(ls, col, delimit)) 
				{
					if (idx == indexCol) 
					{
						_indexCol = col;
					}
					else if (std::find(skipCols.begin(), skipCols.end(), idx) == skipCols.end()) 
					{
						_columns.push_back(col);
					}
					idx++;

				}
			}
			else 
			{
				size_t idx_d{ 0 };
				while (std::getline(ls, col, delimit)) {
					if (std::find(skipCols.begin(), skipCols.end(), idx) == skipCols.end() && indexCol != idx) 
					{
						_columns.push_back(std::to_string(idx_d));
						idx_d++;
					}
					idx++;
				}
			}

			
			_ncols = _columns.size();
		}


		void readRow(std::string& line, char delimit, int indexCol, std::vector<int>& skipCols, std::int64_t& curPos)
		{
			std::stringstream ds(line);
			std::string line_2;
			int skipColTracker{ 0 };
			int colCount{ 0 };
			while (std::getline(ds, line_2, delimit)) 
			{
				if (skipColTracker == indexCol) 
				{
					_index.push_back(line_2);
				}
				else if (std::find( skipCols.begin(), skipCols.end(), skipColTracker) == skipCols.end())
				{
					size_t ln = line_2.length();

					_printWidth = _printWidth > ln ? _printWidth : ln;
					try
					{
						_data[curPos] = T(std::stod(line_2));
					}
					catch (const std::exception&)
					{
						_data[curPos] = T(std::stod("nan"));
					}
					colCount++;
					curPos++;
				}

				if (colCount > _ncols) {
					std::cerr << "Warning: number of columns vary in data file" << std::endl;
					break;
				}

				skipColTracker++;

			}
		}

		void readData(std::string csvFile, char delimit, bool hasHeader, std::vector<int>skipRows, std::vector<int>skipCols, std::int64_t indexCol)
		{
			getFileInfo(csvFile, delimit, hasHeader, skipRows, skipCols, indexCol);

			std::ifstream fileStream;
			std::string line;
			std::int64_t rowPos = 0;
			std::int64_t curPos = 0;
			bool rdh{ !hasHeader };
			fileStream.open(csvFile);

			while (std::getline(fileStream, line)) 
			{
				if (line.empty())
					break;

				if (std::find( skipRows.begin(), skipRows.end(), rowPos) == skipRows.end())
				{
					if (!rdh)
						rdh = true;
					else
						readRow(line, delimit, indexCol, skipCols, curPos);
					
				}
				rowPos++;
			}
			if(_index.size() != _nrows)
			{
				_index.clear();
				for (size_t i{ 0 }; i < _nrows; i++)
				{
					_index.push_back(std::to_string(i));
				}
			}

			fileStream.close();

			if (_index.size() != _nrows || _columns.size()!= _ncols)
			{
				throw std::runtime_error("failed to read file correctly " + csvFile);
			}
		}

		void writeHead(std::ofstream& writeStream, char delimit, bool writeIndex)
		{
			if (writeIndex)
				writeStream << _indexCol << delimit;
			for (size_t i = 0; i < _columns.size(); i++)
			{
				writeStream << _columns[i];
				if (i < (int)_columns.size() - 1)
					writeStream << delimit;
			}
			writeStream << "\n";
		}

		void writeData(std::ofstream& writeStream, char delimit, bool writeIndex)
		{
			for (size_t i = 0; i < _nrows; i++)
			{
				if (writeIndex)
					writeStream << _index[i] << delimit;
				for (size_t j = 0; j < _ncols; j++)
				{
					int idx = i * _ncols + j;
					writeStream << _data[idx];
					if (j < (int)_ncols - 1)
						writeStream << delimit;
				}
				writeStream << "\n";
			}
		}

	public:
		ReadCSV(std::string csvFile, char delimit = ',', bool hasHeader = true, std::vector<int>skipRows = {}, std::vector<int>skipCols = {}, std::int64_t indexCol = -1)
		{
			readData(csvFile, delimit, hasHeader, skipRows, skipCols, indexCol);
		}

		ReadCSV(const MatArray<T>& mat, const std::vector<std::string>& headers = {}, const std::vector<std::string>& index={}, std::string indexCol = "index")
		{
			_nrows = mat.rows();
			_ncols = mat.columns();
			_data = std::vector<T>(_nrows * _ncols);
			_indexCol = indexCol;
			
			if (index.size() < _nrows)
			{
				_index = std::vector<std::string>(_nrows);
				for (size_t i{ 0 }; i < _nrows; i++)
				{
					_index[i] = std::to_string(i);
				}
			}
			else
			{
				_index = index;
			}

			if (headers.size() < _ncols)
			{
				_columns = std::vector<std::string>(_ncols);
				for (size_t i{ 0 }; i < _ncols; i++)
				{
					_columns[i] = std::to_string(i);
				}
			}
			else
			{
				_columns = headers;
			}
			std::copy( mat.begin(), mat.end(), _data.begin());

		}

		ReadCSV(const std::vector<T>& mat, std::int64_t rows, std::int64_t cols, const std::vector<std::string>& headers = {}, const std::vector<std::string>& index = {}, std::string indexCol="index")
		{
			_nrows = rows;
			_ncols = cols;
			_data = std::vector<T>(_nrows * _ncols);
			_indexCol = indexCol;

			if (index.size() < _nrows)
			{
				_index = std::vector<std::string>(_nrows);
				for (size_t i{ 0 }; i < _nrows; i++)
				{
					_index[i] = std::to_string(i);
				}
			}
			else
			{
				_index = index;
			}

			if (headers.size() < _ncols)
			{
				_columns = std::vector<std::string>(_ncols);
				for (size_t i{ 0 }; i < _ncols; i++)
				{
					_columns[i] = std::to_string(i);
				}
			}
			else
			{
				_columns = headers;
			}
			std::copy( mat.begin(), mat.end(), _data.begin());

		}

		void writeCSV(std::string csvFile, char delimit=',', bool writeColNames=true, bool writeIndexNames=false)
		{
			std::ofstream writeStream;
			writeStream.open(csvFile);
			if(writeStream.is_open())
			{
				if (writeColNames)
					writeHead(writeStream, delimit, writeIndexNames);
				writeData(writeStream, delimit, writeIndexNames);
				writeStream.close();
			}
			else
			{
				throw std::runtime_error("Invalid file provided " + csvFile);
			}
		}

		void head(int num=10)
		{
			
			if (num > _nrows) {
				num = _nrows;
			}
			int maxCols = 10;
			if (_ncols > maxCols) {
				int dp;
				for (int i{ 0 }; i < maxCols; i++) {
					dp = (_ncols - maxCols / 2) + (i % (maxCols / 2));
					if (i < maxCols/2)
						dp = i;

					std::cout << std::setw(_printWidth) << _columns[dp];
					if (i == (maxCols/2) - 1)
						std::cout << std::setw(_printWidth) << " ......... ";

				}
				std::cout << std::endl;
				int cl;
				int indx;
				for (int j{ 0 }; j < num; j++) {
					for (int i{ 0 }; i < maxCols; i++) {
						if (i < maxCols/2) {
							cl = i;
						}
						else if (i >= maxCols/2) {
							cl = (_ncols - maxCols/2) + (i % (maxCols/2));
						}

						indx = (j * _ncols) + cl;

						if (cl == 0)
							std::cout << std::endl;
						std::cout << std::setw(_printWidth) << _data[indx];
						if (i == (maxCols / 2) - 1)
							std::cout << std::setw(_printWidth) << " ......... ";
					}
				}

			}
			else {
				for (auto i : _columns)
					std::cout << std::setw(_printWidth) << i;
				for (int i{ 0 }; i < num * _ncols; i++) {
					int cl = i % _ncols;
					int rw = (i - cl) / _ncols;

					int indx = (rw * _ncols) + cl;
					if (cl == 0)
						std::cout << std::endl;
					std::cout << std::setw(_printWidth) << _data[indx];
				}

			}

			std::cout << std::endl;

			std::cout << _nrows << " rows " << _ncols << " columns " << std::endl;

		}


		void tail(int num=10)
		{
			

			if (num > _nrows) {
				num = _nrows;
			}
			int maxCols = 10;
			if (_ncols > maxCols) {
				int dp;
				for (int i{ 0 }; i < maxCols; i++) {
					dp = (_ncols - maxCols / 2) + (i % (maxCols / 2));
					if (i < maxCols/2)
						dp = i;

					std::cout << std::setw(_printWidth) << _columns[dp];
					if (i == (maxCols/2) - 1)
						std::cout << std::setw(_printWidth) << " ......... ";

				}
				std::cout << std::endl;
				int cl;
				int rw;
				int indx;
				for (int j{ 0 }; j < num; j++) {
					for (int i{ 0 }; i < maxCols; i++) {
						if (i < maxCols / 2) {
							cl = i;
						}
						else if (i >= maxCols / 2) {
							cl = (_ncols - maxCols / 2) + (i % (maxCols / 2));
						}
						rw = (_nrows - num) + j;
						indx = (rw * _ncols) + cl;

						if (cl == 0)
							std::cout << std::endl;
						std::cout << std::setw(_printWidth) << _data[indx];
						if (i == (maxCols / 2) - 1)
							std::cout << std::setw(_printWidth) << " ......... ";
					}
				}

			}
			else {
				for (auto i : _columns)
					std::cout << std::setw(_printWidth) << i;
				for (int i{ 0 }; i < num * _ncols; i++) {
					int cl = i % _ncols;
					int rw = (_nrows - num) + ((i - cl) / _ncols);

					int indx = (rw * _ncols) + cl;
					if (cl == 0)
						std::cout << std::endl;
					std::cout << std::setw(_printWidth) << _data[indx];
				}

			}

			std::cout << std::endl;

			std::cout << _nrows << " rows " << _ncols << " columns " << std::endl;
		}


		/*MatArray<T> toMatArray()
		{
			return MatArray<T>(_data, _nrows, _ncols);
		}*/

		auto operator[](size_t _idx)
		{
			if(_idx  >= _columns.size())
				throw std::runtime_error("column index out of range\n");
			std::vector<T> ret_data(_nrows, T(0));

			for (size_t i {0}; i < _nrows; i++)
			{
				size_t iLoc = i * _ncols + _idx;
				ret_data[i] = _data[iLoc];
			}
			return ReadCSV<T>(std::move(ret_data), _nrows, 1, { _columns[_idx] }, _index, _indexCol);
		}

		auto operator[](std::string _col)
		{
			size_t _idx = std::distance(std::find( _columns.begin(), _columns.end(), _col), _columns.begin());
			if (_idx >= _columns.size())
				throw std::runtime_error("column index out of range\n");
			
			return operator[](_idx);
		}

		auto operator[](std::vector<size_t>_cols)
		{
			std::vector<std::string> cls;
			for (size_t i{0}; i < _cols.size(); i++)
			{
				if (_cols[i] >= _columns.size())
					throw std::runtime_error("column index out of range\n");
				cls.push_back(_columns[_cols[i]]);
			}
				
			std::vector<T> ret_data(_nrows * _cols.size(), T(0));

			for (size_t i{ 0 }; i < _nrows; i++)
			{
				for(size_t j{0}; j < _cols.size(); j++)
				{
					size_t iLoc = i * _ncols + _cols[j];
					size_t iloc_2 = i * _cols.size() + j;
					ret_data[iloc_2] = _data[iLoc];
				}
				
			}
			return ReadCSV<T>(std::move(ret_data), _nrows, _cols.size(), std::move(cls), _index, _indexCol);
		}

		auto operator[](std::vector<std::string>_cols)
		{
			std::vector<size_t> cls(_cols.size(), 0);
			for (size_t i{ 0 }; i < _cols.size(); i++)
			{
				size_t _idx = std::distance(std::find( _columns.begin(), _columns.end(), _cols[i]), _columns.begin());
				if (_idx >= _columns.size())
					throw std::runtime_error("column index out of range\n");
				cls[i] = _idx;
			}

			std::vector<T> ret_data(_nrows * _cols.size(), T(0));

			for (size_t i{ 0 }; i < _nrows; i++)
			{
				for (size_t j{ 0 }; j < _cols.size(); j++)
				{
					size_t iLoc = i * _ncols + cls[j];
					size_t iloc_2 = i * _cols.size() + j;
					ret_data[iloc_2] = _data[iLoc];
				}

			}
			return ReadCSV<T>(std::move(ret_data), _nrows, _cols.size(), std::move(_cols), _index, _indexCol);
		}
		auto begin() { return _data.begin(); }
		auto end() { return _data.end(); }

		std::vector<std::string> dataColumns() { return _columns; }
		std::vector<std::string> dataIndex() { return _index; }

		T& at(size_t rw, size_t col)
		{
			size_t idx = rw * _ncols + col;
			return _data[idx];
		}

		T at(size_t rw, size_t col) const
		{
			size_t idx = rw * _ncols + col;
			return _data[idx];
		}

		size_t columnSize()
		{
			return _ncols;
		}
		size_t rowSize()
		{
			return _nrows;
		}

		size_t columnSize() const
		{
			return _ncols;
		}
		size_t rowSize() const
		{
			return _nrows;
		}

	private:
		std::int64_t _nrows;
		std::int64_t _ncols;
		std::vector<T> _data;

		std::vector<std::string>_columns;
		std::vector<std::string>_index;
		std::string _indexCol{ "index" };

		size_t _printWidth{ 10 };
	};

	template<typename T>
	inline void saveCSV(std::string fileName, const MatArray<T>& mat, char delimit=',', const std::vector<std::string>& columnNames = {}, 
		const std::vector<std::string>& rowIndicies = {}, std::string indexColName = "index")
	{
		bool writeHeader = !columnNames.empty();
		bool wrideIndex = !rowIndicies.empty();
		ReadCSV<T> _csv = ReadCSV<T>(mat, columnNames, rowIndicies, indexColName);
		_csv.writeCSV(fileName, delimit, writeHeader, wrideIndex);
	}

	template<typename T>
	inline void saveCSV(std::string fileName, const std::vector<T>& mat, int nrows, int ncols, char delimit = ',', const std::vector<std::string>& columnNames = {},
		const std::vector<std::string>& rowIndicies = {}, std::string indexColName = "index")
	{
		bool writeHeader = !columnNames.empty();
		bool wrideIndex = !rowIndicies.empty();
		ReadCSV<T> _csv = ReadCSV<T>(mat, nrows, ncols, columnNames, rowIndicies, indexColName);
		_csv.writeCSV(fileName, delimit, writeHeader, wrideIndex);
	}

	inline std::vector<std::string> getCSVHeader(std::string fileName, char delimit = ',')
	{
		std::vector<std::string> retVals{};

		std::ifstream fileStream;
		std::string line;
		fileStream.open(fileName);
		if (fileStream.is_open())
		{
			std::getline(fileStream, line);

			std::string col = "";
			std::stringstream ls(line);

			
			while (std::getline(ls, col, delimit))
			{
				retVals.push_back(col);
			}
			
			
			fileStream.close();
		}
		else
		{
			throw std::runtime_error("unable to open file: " + fileName);
		}

		return retVals;

	}
}

