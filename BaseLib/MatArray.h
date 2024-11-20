#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string>
#include <execution>
#include <ranges>
#include <iterator>
#include <type_traits>
#include <sstream>




namespace nims_n {
    template<typename T>
    class MatArray;
//#define PARALLEL std::execution::par_unseq
    struct stride
    {
        size_t jumpSize{ 1 };
        size_t jumpCount{ 1 };
        size_t oneBlockSize{ 1 };
        size_t sbJump_s{ 0 };
        size_t sbJump_c{ 1 };
        size_t sbJump_to{ 1 };


        stride(size_t js, size_t jc, size_t ob) : jumpSize{ js }, jumpCount{ jc }, oneBlockSize{ ob }
        {

        }
        stride() = default;
    };

    template<typename T>
    requires  std::bidirectional_iterator<T>
    class MatrixView
    {
        size_t nrows{ 1 };
        size_t ncols{ 1 };
        size_t viewSize{ 1 };

        T viewBegin;
        stride mat_stride;
        T viewEnd;

        template<typename M>
        friend class MatArray;
    

        
        
        explicit MatrixView(const T& begin, size_t rw, size_t cl, stride sd) :
            nrows{ rw }, ncols{ cl }, viewSize{ nrows * ncols }, viewBegin{ begin }, mat_stride{ sd },
            viewEnd{ viewBegin }
        {
            viewEnd += (mat_stride.jumpCount - (size_t)1) * mat_stride.jumpSize + mat_stride.oneBlockSize;
        }

    public:
        typedef typename T value_type;
        //typedef typename MatrixView<T> type;

        inline size_t size() const
        {
            return viewSize;
        }


        inline size_t rows() const { return nrows; }
        inline size_t columns() const { return ncols; }
        inline size_t tubes() const { return 1; }

        class iterator
        {
            T cCur;
            T cbegin;
            T cend;
            stride vs;

            size_t jumpIndex{ 0 };
            size_t blockIndex{ 1 };
            size_t jumpSwitch{ 0 };
            size_t blockAdvance{ 1 };

        public:
            using difference_type = ptrdiff_t;
            using reference = T&;
            using pointer = T*;
            using value_type = std::remove_reference_t<T>;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator(T cb, T ce, T it, stride mat_s) : cCur{ it }, cbegin{ cb }, cend{ ce }, vs{ mat_s }
            {}
            iterator& operator ++() {

                if (blockIndex == vs.oneBlockSize && jumpIndex < vs.jumpCount - (size_t)1) {
                    blockIndex = 0;
                    jumpIndex++;
                    jumpSwitch = 1;
                }

                cCur += jumpSwitch * (vs.jumpSize - vs.oneBlockSize) + 1;
                blockIndex++;
                jumpSwitch = 0;


                return *this;
            }

            iterator& operator --() {

                if (blockIndex > vs.oneBlockSize && jumpIndex < vs.jumpCount - (size_t)1) {
                    blockIndex = 1;
                    jumpIndex++;
                    jumpSwitch = 1;
                }

                cCur -= (vs.jumpSize - vs.oneBlockSize) * jumpSwitch + 1;
                blockIndex++;
                jumpSwitch = 0;

                return *this;
            }

            iterator& operator +(size_t n) {
                for (size_t i{ 0 }; i < n; i++)
                    ++*this;

                return *this;
            }

            iterator& operator +=(size_t n) {
                for (size_t i{ 0 }; i < n; i++)
                    ++*this;

                return *this;
            }

            auto& operator*() const
            {
                return *cCur;
            }
            auto& operator*()
            {
                return *cCur;
            }

            auto* operator->() const
            {
                return &*cCur;
            }

            bool operator ==(const iterator& rhs) const
            {
                return cCur == rhs.cCur;
            }
            bool operator !=(const iterator& rhs) const
            {
                return cCur != rhs.cCur;
            }

            auto* operator->() 
            {
                return &*cCur;
            }

            bool operator ==(const iterator& rhs) 
            {
                return cCur == rhs.cCur;
            }
            bool operator !=(const iterator& rhs)
            {
                return cCur != rhs.cCur;
            }
        };

        auto begin() const
        {
            return iterator(viewBegin, viewEnd, viewBegin, mat_stride);
        }

        auto end() const
        {
            return iterator(viewBegin, viewEnd, viewEnd, mat_stride);
        }

        auto begin() 
        {
            return iterator(viewBegin, viewEnd, viewBegin, mat_stride);
        }

        auto end() 
        {
            return iterator(viewBegin, viewEnd, viewEnd, mat_stride);
        }

        auto columnSlice(size_t cl) const 
        {
            stride vs(mat_stride.sbJump_s, mat_stride.sbJump_c, 1);
            vs.sbJump_c = nrows;
            vs.sbJump_s = 0;
            T iter = viewBegin;
            iter += mat_stride.sbJump_to * cl;

            return MatrixView<T>(iter, nrows, 1, vs);
        }

        auto f_mat_view() { return 1; }
        auto f_mat_view()const { return 1; }
    };


    template<typename T>
    class MatArray
    {
        using vec = std::vector<T>;

        size_t nrows{ 1 };
        size_t ncols{ 1 };
        size_t ntubes{ 1 };

        size_t data_size{ 0 };
        vec data{ T(1) };

        void normalize(size_t axis = 0)
        {
            if (axis == 0)
            {
                auto _begin = begin();
                auto _end = _begin + ncols;
                for (size_t i{ 0 }; i < nrows; i++)
                {
                    T maxVal = *std::max_element(_begin, _end);
                    std::transform(_begin, _end, _begin, [maxVal](const T& x) {return x / maxVal; });
                    if (i < nrows - (size_t)1) {
                        _begin += ncols;
                        _end += ncols;
                    }

                }
            }
            else
            {
                transpose();

                auto _begin = begin();
                auto _end = _begin + ncols;
                for (size_t i{ 0 }; i < nrows; i++)
                {
                    T maxVal = *std::max_element(_begin, _end);
                    std::transform(_begin, _end, _begin, [maxVal](const T& x) {return x / maxVal; });
                    if (i < nrows - (size_t)1) {
                        _begin += ncols;
                        _end += ncols;
                    }
                }

                transpose();
            }
        }

    public:
        typedef typename T value_type;
        typedef typename MatArray<T> type;

        MatArray(size_t nr, size_t nc, size_t nt = 1) :
            nrows{ nr }, ncols{ nc }, ntubes{ nt }
        {
            data_size = ntubes * nrows * ncols;
            data.resize(data_size);
            std::fill(data.begin(), data.end(), T(1));
        }
        template<typename D>
        //requires std::is_convertible_v<D, vec>
        MatArray(D&& _data, size_t nr, size_t nc, size_t nt = 1) :
            nrows{ nr }, ncols{ nc }, ntubes{ nt }, data{ std::forward<D>(_data) }
        {
            data_size = ntubes * nrows * ncols;
            if (data_size != data.size())
                throw std::runtime_error("matrix size mismatch the size of the data passed");
        }

        //template<typename D>
        //requires std::is_convertible_v<D, vec>
        /*MatArray(D&& _data) :
            data{ std::forward<D>(_data) }
        {
            ncols = data.size();
            data_size = ntubes * nrows * ncols;
            if (data_size != data.size())
                throw std::runtime_error("matrix size mismatch the size of the data passed");
        }*/
        template<typename Vw>
        MatArray(MatrixView<Vw> matV) : nrows{ matV.rows() }, ncols{ matV.columns() }
        {
            data_size = ntubes * nrows * ncols;
            data = std::vector<std::remove_cv_t<std::remove_reference_t<decltype(*matV.begin())>>>(data_size);
            size_t i{ 0 };
            for (auto v = matV.begin(); v != matV.end(); ++v) {
                data[i] = *v;
                i++;
            }


        }


        MatArray() = default;


        auto& operator [](size_t index) {
            return data[index];
        }

        auto operator [](size_t index) const {
            return data[index];
        }

        void setValue(T value) {
            std::fill(data.begin(), data.end(), value);
        }


        T at(size_t rw = 0, size_t cl = 0, size_t tb = 0) const
        {
            size_t idx = tb * nrows * ncols + rw * ncols + cl;
            return data[idx];
        }

        T& at(size_t rw = 0, size_t cl = 0, size_t tb = 0)
        {
            size_t idx = tb * nrows * ncols + rw * ncols + cl;
            return data[idx];
        }

        inline void print(std::ostream& os = std::cout) const {

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t rw{ 0 }; rw < nrows; rw++) {
                    for (size_t cl{ 0 }; cl < ncols; cl++) {
                        size_t idx = tb * nrows * ncols + rw * ncols + cl;
                        os << data[idx] << "\t";
                    }
                    os << "\n";
                }

                os << "\n";
            }
            os << "\n";


        }

        auto columnSlice(size_t col)  const {
            if (col >= ncols)
                throw std::runtime_error("index out of range. max columns = " +
                    std::to_string(ncols) + " but " + std::to_string(col) + " was passed");

            stride vs;
            vs.jumpCount = ntubes * nrows;
            vs.oneBlockSize = 1;
            vs.jumpSize = ncols;
            vs.sbJump_c = ntubes;
            vs.sbJump_s = nrows * ncols;
            vs.sbJump_to = ncols;
            auto iter = begin();
            iter += col;

            return MatrixView<decltype(iter)>(iter, nrows, ntubes, vs);
        }

        auto tubeSlice(size_t tube)  const {
            if (tube >= ntubes)
                throw std::runtime_error("index out of range. max tubes = " +
                    std::to_string(ntubes) + " but " + std::to_string(tube) + " was passed");

            stride vs(0, 1, ncols * nrows);
            vs.sbJump_c = nrows;
            vs.sbJump_s = ncols;
            size_t jump = nrows * ncols * tube;
            auto iter = begin();
            iter += jump;

            return MatrixView<decltype(iter)>(iter, nrows, ncols, vs);
        }

        MatArray<T> columnSliceMat(size_t col) const
        {
            std::vector<T> ret_data(nrows * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t rw{ 0 }; rw < nrows; rw++) {
                    size_t idx = tb * nrows * ncols + rw * ncols + col;
                    size_t idx_r = rw * ntubes + tb;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), nrows, ntubes);
        }

        MatArray<T> columnSliceMat(size_t col)
        {
            std::vector<T> ret_data(nrows * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t rw{ 0 }; rw < nrows; rw++) {
                    size_t idx = tb * nrows * ncols + rw * ncols + col;
                    size_t idx_r = rw * ntubes + tb;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), nrows, ntubes);
        }

        MatArray<T> columnSliceMat_t(size_t col)
        {
            std::vector<T> ret_data(nrows * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t rw{ 0 }; rw < nrows; rw++) {
                    size_t idx = tb * nrows * ncols + rw * ncols + col;
                    size_t idx_r = tb * nrows + rw;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ntubes, nrows);
        }

        MatArray<T> columnSliceMat_t(size_t col) const
        {
            std::vector<T> ret_data(nrows * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t rw{ 0 }; rw < nrows; rw++) {
                    size_t idx = tb * nrows * ncols + rw * ncols + col;
                    size_t idx_r = tb * nrows + rw;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ntubes, nrows);
        }

        MatArray<T> rowSliceMat(size_t row) const
        {
            std::vector<T> ret_data(ncols * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tb * nrows * ncols + row * ncols + cl;
                    size_t idx_r = tb * ncols + cl;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ntubes, ncols);
        }

        MatArray<T> rowSliceMat(size_t row)
        {
            std::vector<T> ret_data(ncols * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tb * nrows * ncols + row * ncols + cl;
                    size_t idx_r = tb * ncols + cl;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ntubes, ncols);
        }

        MatArray<T> rowSliceMat_t(size_t row) const
        {
            std::vector<T> ret_data(ncols * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tb * nrows * ncols + row * ncols + cl;
                    size_t idx_r = cl * ntubes + tb;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ncols, ntubes);
        }

        MatArray<T> rowSliceMat_t(size_t row)
        {
            std::vector<T> ret_data(ncols * ntubes);

            for (size_t tb{ 0 }; tb < ntubes; tb++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tb * nrows * ncols + row * ncols + cl;
                    size_t idx_r = cl * ntubes + tb;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ncols, ntubes);
        }

        MatArray<T> tubeSliceMat(size_t tube) const
        {
            std::vector<T> ret_data(ncols * nrows);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tube * nrows * ncols + rw * ncols + cl;
                    size_t idx_r = rw * nrows + cl;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), nrows, ncols);
        }

        MatArray<T> tubeSliceMat(size_t tube)
        {
            std::vector<T> ret_data(ncols * nrows);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tube * nrows * ncols + rw * ncols + cl;
                    size_t idx_r = rw * ncols + cl;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), nrows, ncols);
        }

        MatArray<T> tubeSliceMat_t(size_t tube) const
        {
            std::vector<T> ret_data(ncols * nrows);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tube * nrows * ncols + rw * ncols + cl;
                    size_t idx_r = cl * ncols + rw;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ncols, nrows);
        }

        MatArray<T> tubeSliceMat_t(size_t tube) 
        {
            std::vector<T> ret_data(ncols * nrows);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    size_t idx = tube * nrows * ncols + rw * ncols + cl;
                    size_t idx_r = cl * ncols + rw;
                    ret_data[idx_r] = data[idx];
                }

            }
            return MatArray<T>(std::move(ret_data), ncols, nrows);
        }

        MatArray<T>getSlice(std::vector<size_t> idxs, std::int64_t axis=0) const
        {
            if (axis == 0)
            {
                size_t rws = idxs.size();
                std::vector<T> ret_data(ncols * rws * ntubes);

                for (size_t tb{ 0 }; tb < ntubes; tb++) {
                    for (size_t rw{ 0 }; rw < rws; rw++) {
                        for (size_t cl{ 0 }; cl < ncols; cl++) {
                            size_t idx = tb * nrows * ncols + idxs[rw] * ncols + cl;
                            size_t idx_r = tb * rws * ncols + rw * ncols + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), rws, ncols, ntubes);

            }
            else if(axis == 1)
            {
                size_t cls = idxs.size();
                std::vector<T> ret_data(nrows * cls * ntubes);

                for (size_t tb{ 0 }; tb < ntubes; tb++) {
                    for (size_t rw{ 0 }; rw < nrows; rw++) {
                        for (size_t cl{ 0 }; cl < cls; cl++) {
                            size_t idx = tb * nrows * ncols + rw * ncols + idxs[cl];
                            size_t idx_r = tb * nrows * cls + rw * cls + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), nrows, cls, ntubes);
            }
            else
            {
                size_t tbs = idxs.size();
                std::vector<T> ret_data(nrows * tbs * ncols);

                for (size_t tb{ 0 }; tb < tbs; tb++) {
                    for (size_t rw{ 0 }; rw < nrows; rw++) {
                        for (size_t cl{ 0 }; cl < ncols; cl++) {
                            size_t idx = idxs[tb] * nrows * ncols + rw * ncols + cl;
                            size_t idx_r = tb * nrows * ncols + rw * ncols + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), nrows, ncols, tbs);
            }
        }

        MatArray<T>getSlice(std::vector<size_t> idxs, std::int64_t axis = 0)
        {
            if (axis == 0)
            {
                size_t rws = idxs.size();
                std::vector<T> ret_data(ncols * rws * ntubes);

                for (size_t tb{ 0 }; tb < ntubes; tb++) {
                    for (size_t rw{ 0 }; rw < rws; rw++) {
                        for (size_t cl{ 0 }; cl < ncols; cl++) {
                            size_t idx = tb * nrows * ncols + idxs[rw] * ncols + cl;
                            size_t idx_r = tb * rws * ncols + rw * ncols + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), rws, ncols, ntubes);

            }
            else if (axis == 1)
            {
                size_t cls = idxs.size();
                std::vector<T> ret_data(nrows * cls * ntubes);

                for (size_t tb{ 0 }; tb < ntubes; tb++) {
                    for (size_t rw{ 0 }; rw < nrows; rw++) {
                        for (size_t cl{ 0 }; cl < cls; cl++) {
                            size_t idx = tb * nrows * ncols + rw * ncols + idxs[cl];
                            size_t idx_r = tb * nrows * cls + rw * cls + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), nrows, cls, ntubes);
            }
            else
            {
                size_t tbs = idxs.size();
                std::vector<T> ret_data(nrows * tbs * ncols);

                for (size_t tb{ 0 }; tb < tbs; tb++) {
                    for (size_t rw{ 0 }; rw < nrows; rw++) {
                        for (size_t cl{ 0 }; cl < ncols; cl++) {
                            size_t idx = idxs[tb] * nrows * ncols + rw * ncols + cl;
                            size_t idx_r = tb * nrows * ncols + rw * ncols + cl;
                            ret_data[idx_r] = data[idx];
                        }
                    }

                }
                return MatArray<T>(std::move(ret_data), nrows, ncols, tbs);
            }
        }

        auto rowSlice(size_t row)  const {
            if (row >= nrows)
                throw std::runtime_error("index out of range. max rows = " +
                    std::to_string(nrows) + " but " + std::to_string(row) + " was passed");
            stride vs;
            vs.jumpCount = ntubes;
            vs.oneBlockSize = ncols;
            vs.jumpSize = nrows * ncols;
            vs.sbJump_c = ntubes;
            vs.sbJump_s = nrows * ncols;
            auto iter = begin();
            iter += row * ncols;

            return MatrixView<decltype(iter)>(iter, ntubes, ncols, vs);
        }

        auto columnSlice(size_t col)
        {
            if (col >= ncols)
                throw std::runtime_error("index out of range. max columns = " +
                    std::to_string(ncols) + " but " + std::to_string(col) + " was passed");

            stride vs;
            vs.jumpCount = ntubes * nrows;
            vs.oneBlockSize = 1;
            vs.jumpSize = ncols;
            vs.sbJump_c = ntubes;
            vs.sbJump_s = nrows * ncols;
            vs.sbJump_to = ncols;
            auto iter = begin();
            iter += col;

            return MatrixView<decltype(iter)>(iter, nrows, ntubes, vs);
        }

        auto tubeSlice(size_t tube)
        {
            if (tube >= ntubes)
                throw std::runtime_error("index out of range. max tubes = " +
                    std::to_string(ntubes) + " but " + std::to_string(tube) + " was passed");

            stride vs(0, 1, ncols * nrows);
            vs.sbJump_c = nrows;
            vs.sbJump_s = ncols;
            size_t jump = nrows * ncols * tube;
            auto iter = begin();
            iter += jump;

            return MatrixView<decltype(iter)>(iter, nrows, ncols, vs);
        }

        
        auto rowSlice(size_t row) 
        {
            if (row >= nrows)
                throw std::runtime_error("index out of range. max rows = " +
                    std::to_string(nrows) + " but " + std::to_string(row) + " was passed");
            stride vs;
            vs.jumpCount = ntubes;
            vs.oneBlockSize = ncols;
            vs.jumpSize = nrows * ncols;
            vs.sbJump_c = ntubes;
            vs.sbJump_s = nrows * ncols;
            auto iter = begin();
            iter += row * ncols;

            return MatrixView<decltype(iter)>(iter, ntubes, ncols, vs);
        }

        void transpose() {
            std::vector<T> _temp(data_size);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    const size_t tindex = (rw * ncols) + cl;
                    const size_t cindex = (cl * nrows) + rw;
                    _temp[cindex] = data[tindex];
                }

            }
            size_t _col = ncols;
            ncols = nrows;
            nrows = _col;
            std::copy(_temp.begin(), _temp.end(), data.begin());
        }

        void resetToZeros()
        {
            std::fill(std::execution::par_unseq, data.begin(), data.end(), T(0));
        }

        MatArray<T> getTransposed() const
        {
            std::vector<T> _temp(data_size);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    const size_t tindex = (rw * ncols) + cl;
                    const size_t cindex = (cl * nrows) + rw;
                    _temp[cindex] = data[tindex];
                }

            }

            return MatArray<T>(std::move(_temp), ncols, nrows);
        }

        MatArray<T> getTransposed()
        {
            std::vector<T> _temp(data_size);

            for (size_t rw{ 0 }; rw < nrows; rw++) {
                for (size_t cl{ 0 }; cl < ncols; cl++) {
                    const size_t tindex = (rw * ncols) + cl;
                    const size_t cindex = (cl * nrows) + rw;
                    _temp[cindex] = data[tindex];
                }

            }

            return MatArray<T>(std::move(_temp), ncols, nrows);
        }

        void copyTo(MatArray<T>& other)
        {
            if (other.size() != data_size)
            {
                other.data.resize(data_size);
                other.nrows = nrows;
                other.ncols = ncols;
                other.ntubes = ntubes;
                other.data_size = data_size;
            }
            std::copy( begin(), end(), other.begin());
        }

        void copyTo(MatArray<T>& other) const
        {
            if (other.size() != data_size)
            {
                other.data.resize(data_size);
                other.nrows = nrows;
                other.ncols = ncols;
                other.ntubes = ntubes;
                other.data_size = data_size;
            }
            std::copy(begin(), end(), other.begin());
        }

        
        void rowNorm()
        {
            normalize();
        }

        void columnNorm()
        {
            normalize(1);
        }

        MatArray<T> toDiag()
        {
           
            size_t dim = nrows * ncols;
            std::vector<T>ret_data(dim*dim, T(0));
            
            int count = 0;

            for (int rw{ 0 }; rw < dim; rw++) {
                for (int cl{ 0 }; cl < dim; cl++) {
                    int tindex = (rw * dim) + cl;
                    if (rw == cl) {
                        ret_data[tindex] = data[count];
                        count++;
                    }
                }


            }

            return MatArray<T>(ret_data, dim, dim);
        }

        auto mat() { return 1; }
        auto mat() const { return 1; }
        auto f_mat_view() { return 1; }
        auto f_mat_view() const { return 1; }

        auto begin()
        {
            return data.begin();
        }

        auto end()
        {
            return data.end();
        }

        auto begin() const
        {
            return data.cbegin();
        }

        auto end() const
        {
            return data.cend();
        }

        inline size_t size() const
        {
            return data_size;
        }

        inline size_t rows() const { return nrows; }
        inline size_t columns() const { return ncols; }
        inline size_t tubes() const { return ntubes; }


    };
    template<typename L>
    concept Mat_View = requires(L lhs) {
        lhs.begin();
    };

    template<typename L>
    concept Mat_View2 = requires(L lhs) {
        std::bidirectional_iterator<decltype( lhs.begin())>;
        lhs.f_mat_view();
    };

    template<typename L>
    concept Mat = requires(L lhs) {
        lhs.mat();
    };

    template<typename L>
    concept Numeral = requires(L lhs) {
        std::is_arithmetic_v<L>;
        lhs++;
    };


#ifdef PARALLEL
    auto argMax(const Mat_View auto& arg) {
        return std::distance(arg.begin(), std::max_element(std::execution::par_unseq, arg.begin(), arg.end()));
    }

    auto argMin(const Mat_View auto& arg) {
        return std::distance(arg.begin(), std::min_element(std::execution::par_unseq, arg.begin(), arg.end()));
    }

    auto minElement(const Mat_View auto& arg) {
        return *std::min_element(std::execution::par_unseq, arg.begin(), arg.end());
    }

    auto maxElement(const Mat_View auto& arg) {
        return *std::max_element(std::execution::par_unseq, arg.begin(), arg.end());
    }

    void colNorm(Mat auto& arg, size_t col) {
        auto slice = arg.columnSlice(col);
        auto maxVal = maxElement(slice);

        std::transform(slice.begin(), slice.end(), slice.begin(), [maxVal](auto x) {return x / maxVal; });
    }

    auto operator+ (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x + y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator+ (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x + rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator+ (const Numeral auto& lhs, const  Mat_View auto& rhs) {


        return rhs + lhs;
    }

    void operator+=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x + rhs; });
    }

    void operator+=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x + y; });
    }

    void operator*=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x * rhs; });
    }

    void operator*=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x * y; });
    }
    void operator/=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x / rhs; });
    }

    void operator/=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x / y; });
    }

    void operator-=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x - rhs; });
    }

    void operator-=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x - y; });
    }

    auto operator* (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x * rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator* (const Numeral auto& lhs, const  Mat_View auto& rhs) {


        return rhs * lhs;
    }

    auto operator- (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x - rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator- (const Numeral auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*rhs.begin())>>;
        std::vector<v_type> ret_data(rhs.size());
        std::transform(std::execution::par_unseq, rhs.begin(), rhs.end(), ret_data.begin(), [lhs](const v_type& x) {return lhs - x; });

        return MatArray<v_type>(std::move(ret_data), rhs.rows(), rhs.columns(), rhs.tubes());
    }

    auto operator/ (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x / rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator/ (const Numeral auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*rhs.begin())>>;
        std::vector<v_type> ret_data(rhs.size());
        std::transform(std::execution::par_unseq, rhs.begin(), rhs.end(), ret_data.begin(), [lhs](const v_type& x) {return lhs / x; });

        return MatArray<v_type>(std::move(ret_data), rhs.rows(), rhs.columns(), rhs.tubes());
    }

    auto operator- (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x - y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator* (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x * y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator/ (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(std::execution::par_unseq, lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x / y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    template<typename T>
    MatArray<T> zeros(size_t rw, size_t cl, size_t tb = 1)
    {
        size_t sz = rw * cl * tb;
        std::vector<T> ret_data(sz, T(0));

        return MatArray<T>(std::move(ret_data), rw, cl, tb);
    }

    template<typename T>
    auto operator& (const MatArray<T>& lhs, const MatArray<T>& rhs)
    {
        std::vector<T>ret_data(lhs.rows() * rhs.columns());
        MatArray<T> rhs_copy = rhs.getTransposed();
        for (size_t rw = 0, rw_increment = lhs.columns(); rw < lhs.rows(); rw_increment += lhs.columns(), rw++) {
            auto a_begin = lhs.begin() + (rw_increment - lhs.columns());
            auto a_end = lhs.begin() + rw_increment;

            for (size_t cl = 0, cl_increment = rhs_copy.columns(); cl < rhs_copy.rows(); cl_increment += rhs_copy.columns(), cl++) {
                auto b_begin = rhs_copy.begin() + (cl_increment - rhs_copy.columns());
                size_t idx = rw * rhs.columns() + cl;

                T initVal{ 0.0 };

                ret_data[idx] = std::transform_reduce(std::execution::par_unseq, a_begin,
                    a_end, b_begin, initVal, std::plus<>(), std::multiplies<>());
            }
        }

        return MatArray<T>(std::move(ret_data), lhs.rows(), rhs.columns());
    }

    template<typename T>
    void multiplyMat(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& _out)
    {
        MatArray<T> rhs_copy = rhs.getTransposed();
        for (size_t rw = 0, rw_increment = lhs.columns(); rw < lhs.rows(); rw_increment += lhs.columns(), rw++) {
            auto a_begin = lhs.begin() + (rw_increment - lhs.columns());
            auto a_end = lhs.begin() + rw_increment;

            for (size_t cl = 0, cl_increment = rhs_copy.columns(); cl < rhs_copy.rows(); cl_increment += rhs_copy.columns(), cl++) {
                auto b_begin = rhs_copy.begin() + (cl_increment - rhs_copy.columns());
                size_t idx = rw * rhs.columns() + cl;

                T initVal{ 0.0 };

                _out[idx] = std::transform_reduce(std::execution::par_unseq, a_begin,
                    a_end, b_begin, initVal, std::plus<>(), std::multiplies<>());
            }
        }
    }
#else
    auto argMax(const Mat_View auto& arg) {
        return std::distance(arg.begin(), std::max_element(arg.begin(), arg.end()));
        }

    auto argMin(const Mat_View auto& arg) {
        return std::distance(arg.begin(), std::min_element(arg.begin(), arg.end()));
    }

    auto minElement(const Mat_View auto& arg) {
        return *std::min_element(arg.begin(), arg.end());
    }

    auto maxElement(const Mat_View auto& arg) {
        return *std::max_element(arg.begin(), arg.end());
    }

    void colNorm(Mat auto& arg, size_t col) {
        auto slice = arg.columnSlice(col);
        auto maxVal = maxElement(slice);

        std::transform(slice.begin(), slice.end(), slice.begin(), [maxVal](auto x) {return x / maxVal; });
    }

    auto operator+ (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x + y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator+ (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x + rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator+ (const Numeral auto& lhs, const  Mat_View auto& rhs) {


        return rhs + lhs;
    }

    void operator+=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x + rhs; });
    }

    void operator+=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x + y; });
    }

    void operator*=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x * rhs; });
    }

    void operator*=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x * y; });
    }
    void operator/=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x / rhs; });
    }

    void operator/=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x / y; });
    }

    void operator-=(Mat auto& lhs, const  Numeral auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), lhs.begin(), [rhs](auto x) {return x - rhs; });
    }

    void operator-=(Mat auto& lhs, const  Mat_View auto& rhs) {
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), [](auto x, auto y) {return x - y; });
    }

    auto operator* (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x * rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator* (const Numeral auto& lhs, const  Mat_View auto& rhs) {


        return rhs * lhs;
    }

    auto operator- (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x - rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator- (const Numeral auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*rhs.begin())>>;
        std::vector<v_type> ret_data(rhs.size());
        std::transform(rhs.begin(), rhs.end(), ret_data.begin(), [lhs](const v_type& x) {return lhs - x; });

        return MatArray<v_type>(std::move(ret_data), rhs.rows(), rhs.columns(), rhs.tubes());
    }

    auto operator/ (const Mat_View auto& lhs, const  Numeral auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), ret_data.begin(), [rhs](const v_type& x) {return x / rhs; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator/ (const Numeral auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*rhs.begin())>>;
        std::vector<v_type> ret_data(rhs.size());
        std::transform(rhs.begin(), rhs.end(), ret_data.begin(), [lhs](const v_type& x) {return lhs / x; });

        return MatArray<v_type>(std::move(ret_data), rhs.rows(), rhs.columns(), rhs.tubes());
    }

    auto operator- (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x - y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator* (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x * y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    auto operator/ (const Mat_View auto& lhs, const  Mat_View auto& rhs) {
        using v_type = typename std::remove_cv_t<std::remove_reference_t<decltype(*lhs.begin())>>;
        std::vector<v_type> ret_data(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), ret_data.begin(), [](const v_type& x, const v_type& y) {return x / y; });

        return MatArray<v_type>(std::move(ret_data), lhs.rows(), lhs.columns(), lhs.tubes());
    }

    template<typename T>
    MatArray<T> zeros(size_t rw, size_t cl, size_t tb = 1)
    {
        size_t sz = rw * cl * tb;
        std::vector<T> ret_data(sz, T(0));

        return MatArray<T>(std::move(ret_data), rw, cl, tb);
    }

    template<typename T>
    MatArray<T> getIdentityMat(size_t rw)
    {
        
        MatArray<T> retMat = zeros<T>(rw, rw);
        for (int i = 0; i < rw; i++)
        {
            retMat.at(i, i) = T(1);
        }

        return retMat;
    }

    template<typename T>
    auto operator& (const MatArray<T>& lhs, const MatArray<T>& rhs)
    {
        std::vector<T>ret_data(lhs.rows() * rhs.columns());
        MatArray<T> rhs_copy = rhs.getTransposed();
        for (size_t rw = 0, rw_increment = lhs.columns(); rw < lhs.rows(); rw_increment += lhs.columns(), rw++) {
            auto a_begin = lhs.begin() + (rw_increment - lhs.columns());
            auto a_end = lhs.begin() + rw_increment;

            for (size_t cl = 0, cl_increment = rhs_copy.columns(); cl < rhs_copy.rows(); cl_increment += rhs_copy.columns(), cl++) {
                auto b_begin = rhs_copy.begin() + (cl_increment - rhs_copy.columns());
                size_t idx = rw * rhs.columns() + cl;

                T initVal{ 0.0 };

                ret_data[idx] = std::transform_reduce(a_begin,
                    a_end, b_begin, initVal, std::plus<>(), std::multiplies<>());
            }
        }

        return MatArray<T>(std::move(ret_data), lhs.rows(), rhs.columns());
    }

    template<typename T>
    void multiplyMat(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& _out)
    {
        MatArray<T> rhs_copy = rhs.getTransposed();
        for (size_t rw = 0, rw_increment = lhs.columns(); rw < lhs.rows(); rw_increment += lhs.columns(), rw++) {
            auto a_begin = lhs.begin() + (rw_increment - lhs.columns());
            auto a_end = lhs.begin() + rw_increment;

            for (size_t cl = 0, cl_increment = rhs_copy.columns(); cl < rhs_copy.rows(); cl_increment += rhs_copy.columns(), cl++) {
                auto b_begin = rhs_copy.begin() + (cl_increment - rhs_copy.columns());
                size_t idx = rw * rhs.columns() + cl;

                T initVal{ 0.0 };

                _out[idx] = std::transform_reduce(a_begin,
                    a_end, b_begin, initVal, std::plus<>(), std::multiplies<>());
            }
        }
    }

#endif // PARALLEL

    template<typename T>
    void multDiag(const MatArray<T>& lhs, const MatArray<T>& diag, MatArray<T>& res) {
        int n = lhs.rows();      // Number of rows in matrix A
        int m = diag.rows();   // Number of columns in A and size of diagonal matrix D


        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                res.at(i, j) = lhs.at(i, j) * diag[j];
            }
        }
    }

    template<typename T>
    void multDiag(const MatArray<T>& lhs, const MatArray<T>& diag, MatArray<T>& res, std::vector<size_t>selColumns) 
    {
        int n = lhs.rows();

        for (int j{ 0 }; j < selColumns.size(); j++)
        {
            for (int i{ 0 }; i < n; i++)
            {
                res.at(i, selColumns[j]) = lhs.at(i, selColumns[j]) * diag[selColumns[j]];
            }

        }
    }

    template<typename T>
    void multSelCol(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& res, std::vector<size_t>selColumns, bool jleft=false)
    {
        MatArray<T> lhs_copy = lhs;
        MatArray<T> rhs_copy = rhs.getTransposed();

        
        int m = rhs_copy.rows();
        int n = lhs_copy.rows();
        int lhs_cols = lhs_copy.columns();
        int rhs_cols = rhs_copy.columns();

        if(!jleft)
        {
            if (m != res.columns() && n != res.rows())
                throw std::runtime_error("rows and columns of res matrix incompatible with input rhs and lhs\n");

          

            for (int i = 0; i < n; i++)
            {
                auto lhs_begin = lhs_copy.begin() + i * lhs_cols;
                auto lhs_end = lhs_begin + lhs_cols;

                for (int j : selColumns)
                {
                    auto rhs_begin = rhs_copy.begin() + j * rhs_cols;
                    T initVal{ T(0.0) };

                    res.at(i, j) = std::transform_reduce(lhs_begin,
                        lhs_end, rhs_begin, initVal, std::plus<>(), std::multiplies<>());

                }
            }
        }
        else
        {
            if(rhs_cols < selColumns.size())
                throw std::runtime_error("rows and columns of res matrix incompatible with input rhs and lhs\n");

            

            for (int i{ 0 }; i < n; i++)
            {
                for (int j{ 0 }; j < m; j++)
                {
                    T initVal{ T(0.0) };

                    for (int k{ 0 }; k < selColumns.size(); k++)
                    {
                        int idx_l = i * lhs_cols + selColumns[k];
                        int idx_r = j * rhs_cols + k;

                        initVal += lhs_copy[idx_l] * rhs_copy[idx_r];
                    }

                    res.at(i, selColumns[j]) = initVal;

                }
            }

        }

        
    }
    
    template<typename T>
    void subSelCol(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& res, std::vector<size_t>selColumns)
    {
        MatArray<T> rhs_copy = rhs.getTransposed();
        MatArray<T> lhs_copy = lhs.getTransposed();
        res.transpose();
        int n = rhs_copy.columns();


        for(int i:selColumns)
        {
            auto lhs_begin = lhs_copy.begin() + i*n;
            auto rhs_begin = rhs_copy.begin() + i*n;
            auto res_begin = res.begin() + i*n;
            auto lhs_end = lhs_begin + n;

            std::transform(lhs_begin, lhs_end, rhs_begin, res_begin, [](const T& x, const T& y) {return x - y; });
        }
        res.transpose();
    }
    
    template<typename T>
    T dotProduct(const MatArray<T>& lhs, int col1, int col2, bool trans = true)
    {
        MatArray<T> lhs_copy = lhs;
        if (trans)
            lhs_copy.transpose();

        int n = lhs_copy.columns();


        auto lhs_begin = lhs_copy.begin() + col1 * n;
        auto rhs_begin = lhs_copy.begin() + col2 * n;
        auto lhs_end = lhs_begin + n;
        T initVal{ T(0.0) };
        T res = std::transform_reduce(lhs_begin,
            lhs_end, rhs_begin, initVal, std::plus<>(), std::multiplies<>());

        return res;
    }

    template<typename T>
    MatArray<T> getEuclideanNorm(const MatArray<T>& lhs, const std::vector<size_t>selColumns = {})
    {
        MatArray<T> lhs_copy = lhs.getTransposed();

        if (selColumns.empty())
        {
            int n = lhs_copy.columns();
            int m = lhs_copy.rows();
            std::vector<T> res(m);

            for (int i = 0; i < m; i++)
            {
                auto lhs_begin = lhs_copy.begin() + i * n;
                auto lhs_end = lhs_begin + n;
                T initVal{ T(0.0) };
                res[i] = std::sqrt(std::transform_reduce(lhs_begin,
                    lhs_end, lhs_begin, initVal, std::plus<>(), std::multiplies<>()));
            }

            return MatArray<T>(std::move(res), m, 1);
        }
        else
        {
            int n = lhs_copy.columns();
            int m = selColumns.size();
            std::vector<T> res(m);

            for (int i = 0; i < m; i++)
            {
                auto lhs_begin = lhs_copy.begin() + selColumns[i] * n;
                auto lhs_end = lhs_begin + n;
                T initVal{ T(0.0) };
                res[i] = std::sqrt(std::transform_reduce(lhs_begin,
                    lhs_end, lhs_begin, initVal, std::plus<>(), std::multiplies<>()));
            }
            return MatArray<T>(std::move(res), m, 1);
        }
    }

    template<typename T>
    T getEuclideanNorm(const MatArray<T>& lhs, int column, bool trans=true)
    {
        T dot = dotProduct(lhs, column, column, trans);

        return std::sqrt(dot);
    }

    template<typename T>
    T getMatNorm(const MatArray<T>& lhs)
    {
        T val{ 0 };
        int rows = lhs.rows();
        int cols = lhs.columns();

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                val += lhs.at(i, j) * lhs.at(i, j);
            }
        }
        return std::sqrt(val);
    }
    
    

    template<typename T>
    void addSelCol(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& res, const std::vector<size_t>selColumns = {})
    {
        MatArray<T> rhs_copy = rhs.getTransposed();
        MatArray<T> lhs_copy = lhs.getTransposed();
        res.transpose();
        int n = rhs_copy.columns();


        for (int i : selColumns)
        {
            auto lhs_begin = lhs_copy.begin() + i * n;
            auto rhs_begin = rhs_copy.begin() + i * n;
            auto res_begin = res.begin() + i * n;
            auto lhs_end = lhs_begin + n;

            std::transform(lhs_begin, lhs_end, rhs_begin, res_begin, [](const T& x, const T& y) {return x + y; });
        }
        res.transpose();
    }

    template<typename T>
    void addMat(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& res)
    {
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), 
            res.begin(), [](const T& x, const T& y) {return x + y; });
    }

    template<typename T>
    MatArray<T> multSelCol(const MatArray<T>& lhs, const MatArray<T>& rhs, const std::vector<size_t>selColumns, bool l_t = false, bool r_t = false)
    {
        MatArray<T> lhs_copy;
        MatArray<T> rhs_copy;

        if (l_t)
            lhs_copy = lhs.getTransposed();
        else
            lhs_copy = lhs;
        if (r_t)
            rhs_copy = rhs;
        else
            rhs_copy = rhs.getTransposed();
        int m = rhs_copy.rows();
        int n = lhs_copy.rows();
        int lhs_cols = lhs_copy.columns();
        int rhs_cols = rhs_copy.columns();
        int res_cols = selColumns.size();

        std::vector<T> res(n * res_cols);

        auto lhs_begin = lhs_copy.begin();
        auto lhs_end = lhs_begin + lhs_cols;

        for (int i = 0; i < n; i++)
        {
            auto lhs_begin = lhs_copy.begin() + i * lhs_cols;
            auto lhs_end = lhs_begin + lhs_cols;

            for (int j{ 0 }; j < res_cols; j++)
            {
                auto rhs_begin = rhs_copy.begin() + selColumns[j] * rhs_cols;
                int idx = res_cols * i + j;
                T initVal{ T(0.0) };

                res[idx] = std::transform_reduce(lhs_begin,
                    lhs_end, rhs_begin, initVal, std::plus<>(), std::multiplies<>());

            }
        }

        return MatArray<T>(std::move(res), n, res_cols);
    }

    template<typename T>
    MatArray<T> pMultSelCol(const MatArray<T>& lhs,  const std::vector<size_t>selColumns)
    {
        MatArray<T> lhs_copy;
        MatArray<T> rhs_copy;

            lhs_copy = lhs.getTransposed();
            rhs_copy = lhs.getTransposed();
        
        int lhs_cols = lhs_copy.columns();
        int rhs_cols = rhs_copy.columns();
        int res_cols = selColumns.size();

        std::vector<T> res(res_cols * res_cols);

        for (int i = 0; i < res_cols; i++)
        {
            auto lhs_begin = lhs_copy.begin() + selColumns[i] * lhs_cols;
            auto lhs_end = lhs_begin + lhs_cols;

            for (int j{ 0 }; j < res_cols; j++)
            {
                auto rhs_begin = rhs_copy.begin() + selColumns[j] * rhs_cols;
                int idx = res_cols * i + j;
                T initVal{ T(0.0) };

                res[idx] = std::transform_reduce(lhs_begin,
                    lhs_end, rhs_begin, initVal, std::plus<>(), std::multiplies<>());

            }
        }

        return MatArray<T>(std::move(res), res_cols, res_cols);
    }

    template<typename T>
    void pMultSelCol(const MatArray<T>& lhs, const MatArray<T>& rhs, MatArray<T>& res, const std::vector<size_t>selColumns)
    {
        MatArray<T> lhs_copy;
        MatArray<T> rhs_copy;

        lhs_copy = lhs.getTransposed();
        rhs_copy = rhs.getTransposed();

        int m = rhs_copy.rows();
        int n = lhs_copy.rows();
        int lhs_cols = lhs_copy.columns();
        int rhs_cols = rhs_copy.columns();
        int res_cols = selColumns.size();


        for (int i = 0; i < res_cols; i++)
        {
            auto lhs_begin = lhs_copy.begin() + selColumns[i] * lhs_cols;
            auto lhs_end = lhs_begin + lhs_cols;

            for (int j{ 0 }; j < res_cols; j++)
            {
                auto rhs_begin = rhs_copy.begin() + selColumns[j] * rhs_cols;
                int idx = res_cols * i + j;
                T initVal{ T(0.0) };

                res[idx] = std::transform_reduce(lhs_begin,
                    lhs_end, rhs_begin, initVal, std::plus<>(), std::multiplies<>());

            }
        }

    }


    template<typename T>
    MatArray<T> lobpcg_gramA(const MatArray<T>& lambda, const MatArray<T>& matX,
        const MatArray<T>& matAW, const MatArray<T>& matWj, const std::vector<size_t>selColumns)
    {
        int m = lambda.size();
        int sizeW = selColumns.size();
        int gramN = m +  sizeW;
        int gramM = gramN;
        int gramRow{ 0 };
        int gramCol{ 0 };

        MatArray<T> gramA = zeros<T>(gramN, gramM);

        for (int i = 0; i < m; i++)
        {
            gramA.at(i, i) = lambda[i];
        }

        gramCol += m;

        MatArray<T> gramTmp = multSelCol(matX, matAW, selColumns, true);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i, gramCol + j) = gramTmp.at(i, j);

            }
        }

        gramRow += m;
        gramTmp = zeros<T>(sizeW, sizeW);
        pMultSelCol<T>(matWj, matAW, gramTmp, selColumns);
        //gramTmp = (gramTmp.getTransposed() + gramTmp) / T(2);

        for (int i = 0; i < sizeW; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i + gramRow, gramCol + j) = gramTmp.at(i, j);

            }
        }
        //gramA = (gramA.getTransposed() + gramA) / T(2);
        for (int i = 0; i < gramN; i++)
        {
            for (int j = i + 1; j < gramN; j++)
            {
                gramA.at(j, i) = gramA.at(i, j);
            }
        }
        return gramA;
    }

    template<typename T>
    MatArray<T> lobpcg_gramA(const MatArray<T>& lambda, const MatArray<T>& matX,
        const MatArray<T>& matAW, const MatArray<T>& matWj, const MatArray<T>& matAP, const MatArray<T>& matP, const std::vector<size_t>selColumns)
    {
        int m = lambda.size();
        int sizeW = selColumns.size();
        int gramN = m + 2 * sizeW;
        int gramM = gramN;
        int gramRow{ 0 };
        int gramCol{ 0 };

        MatArray<T> gramA = zeros<T>(gramN, gramM);

        for (int i = 0; i < m; i++)
        {
            gramA.at(i, i) = lambda[i];
        }

        gramCol += m;

        MatArray<T> gramTmp = multSelCol(matX, matAW, selColumns, true);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i, gramCol + j) = gramTmp.at(i, j);

            }
        }

        gramRow += m;
        gramTmp = zeros<T>(sizeW, sizeW);
        pMultSelCol<T>(matWj, matAW, gramTmp, selColumns);
        //gramTmp = (gramTmp.getTransposed() + gramTmp) / T(2);

        for (int i = 0; i < sizeW; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i + gramRow, gramCol + j) = gramTmp.at(i, j);

            }
        }

        gramRow -= m;
        gramCol += sizeW;
        gramTmp = multSelCol(matX, matAP, selColumns, true);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i + gramRow, gramCol + j) = gramTmp.at(i, j);

            }
        }

        gramRow += m;
        gramTmp = zeros<T>(sizeW, sizeW);
        pMultSelCol<T>(matWj, matAP, gramTmp, selColumns);
        //gramTmp = (gramTmp.getTransposed() + gramTmp) / T(2);

        for (int i = 0; i < sizeW; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i + gramRow, gramCol + j) = gramTmp.at(i, j);

            }
        }

        gramRow += sizeW;
        gramTmp = zeros<T>(sizeW, sizeW);
        pMultSelCol<T>(matP, matAP, gramTmp, selColumns);
        //gramTmp = (gramTmp.getTransposed() + gramTmp) / T(2);

        for (int i = 0; i < sizeW; i++)
        {
            for (int j = 0; j < sizeW; j++)
            {
                gramA.at(i + gramRow, gramCol + j) = gramTmp.at(i, j);

            }
        }

        //gramA = (gramA.getTransposed() + gramA) / T(2);

        for (int i = 0; i < gramN; i++)
        {
            for (int j = i + 1; j < gramN; j++)
            {
                gramA.at(j, i) = gramA.at(i, j);
            }
        }

        return gramA;
    }
    
    template<typename T>
    void lobpcg_partition(int startR, int endR, const MatArray<T>& gramA, const MatArray<T>& lhs, 
                             MatArray<T>& res, const std::vector<size_t>selColumns)
    {
        MatArray<T> gramCopy = gramA.getTransposed();
        int gCols = gramCopy.columns();
        int lhsRows = lhs.rows();
        int lhsCols = lhs.columns();
        int rhsCols = res.columns();

        for (int i = 0; i < lhsRows; i++)
        {
            for (int j = startR, r=0; j < endR; r++, j++)
            {
                T initVal = T(0);
                for (int k = 0; k < selColumns.size(); k++)
                {
                    int idx_gr = startR * gCols + k;
                    int idx_lhs = i * lhsCols + selColumns[k];

                    initVal += gramA[idx_gr] * lhs[idx_lhs];
                }
                int idx_res = i * rhsCols + r;
                res[idx_res] = initVal;
            }
        }
    }

    template<typename T>
    void lobpcg_partition(int startR, int endR, const MatArray<T>& gramA, const MatArray<T>& lhs,
        MatArray<T>& res)
    {
        MatArray<T> gramCopy = gramA.getTransposed();
        int gCols = gramCopy.columns();
        int lhsRows = lhs.rows();
        int lhsCols = lhs.columns();
        int rhsCols = res.columns();

        for (int i = 0; i < lhsRows; i++)
        {
            for (int j = startR, r=0; j < endR; r++, j++)
            {
                T initVal = T(0);
                for (int k = 0; k < rhsCols; k++)
                {
                    int idx_gr = startR * gCols + k;
                    int idx_lhs = i * lhsCols + k;

                    initVal += gramA[idx_gr] * lhs[idx_lhs];
                }
                int idx_res = i * rhsCols + r;
                res[idx_res] = initVal;
            }
        }
    }
    
    template<typename T>
    MatArray<T> setZscoreNorm(MatArray<T>& _data, bool trans=false)
    {
        if (trans)
            _data.transpose();
        int rows = _data.rows();
        int cols = _data.columns();
        MatArray<T> meanAndSd = zeros<T>(rows, 2);


        auto mnsdFunc = [](int start, int end, const MatArray<T>& _dt) ->std::vector<T> {
            auto _begin = _dt.begin() + start;
            auto _end = _begin + end;
            T length = (T)std::distance(_begin, _end);

            T retMean = std::reduce(_begin, _end) / length;
            T initVal{ 0 };

            T retSD = std::sqrt(std::transform_reduce(_begin,
                _end, _begin, initVal, std::plus<>(), [retMean](const T& x, const T& y) {return (x - retMean)* (y - retMean); })/length);
            return std::vector<T>{retMean, retSD};
        };

       

        for (int i = 0; i < rows; i++)
        {
            int _start = i * cols;
            int _end = _start + cols;

            std::vector<T> mnSd = mnsdFunc(_start, _end, _data);
            meanAndSd.at(i, 0) = mnSd[0];
            meanAndSd.at(i, 1) = mnSd[1];
            
            auto _begin = _data.begin() + _start;
            auto _end_it = _begin + _end;

            std::transform(_begin, _end_it, _begin, [&mnSd](const T& x) {return (x - mnSd[0]) / mnSd[1]; });
        }
        if (trans)
            _data.transpose();
        return meanAndSd;
    }

    template<typename T>
    void resetZscoreNorm(MatArray<T>& _data, const MatArray<T>& mnSd, bool trans = false)
    {
        if (trans)
            _data.transpose();
        int rows = _data.rows();
        int cols = _data.columns();
       
        for (int i = 0; i < rows; i++)
        {
            int _start = i * cols;
            int _end = _start + cols;

          
            auto _begin = _data.begin() + _start;
            auto _end_it = _begin + _end;

            std::transform(_begin, _end_it, _begin, [&mnSd, i](const T& x) {
                                return (x * mnSd.at(i, 1)) + mnSd.at(i, 0); 
                });
        }
        if (trans)
            _data.transpose();
    }

    template<typename T>
    MatArray<T> setMinMaxNorm(MatArray<T>&_data, bool trans=false, T _minVal=0.0, T _maxVal=1.0)
    {
        if (trans)
            _data.transpose();
        int rows = _data.rows();
        int cols = _data.columns();
        MatArray<T> minMax = zeros<T>(rows, 2);

        for (int i = 0; i < rows; i++)
        {
            int _start = i * cols;
            int _end = _start + cols;


            auto _begin = _data.begin() + _start;
            auto _end_it = _begin + _end;
            auto mnMx = std::minmax_element(_begin, _end_it);
            minMax.at(i, 0) = *mnMx.first;
            minMax.at(i, 1) = *mnMx.second;

            std::transform(_begin, _end_it, _begin, [&mnMx, &_minVal, _maxVal](const T& x) {
                            return _minVal + 
                                (x - *mnMx.first) * (_maxVal - _minVal)/ 
                                (*mnMx.second - *mnMx.first); });
        }
        if (trans)
            _data.transpose();
        return minMax;
    }

    template<typename T>
    void resetMinMaxNorm(MatArray<T>&_data, const MatArray<T>& mnMax,  bool trans = false, T _minVal = 0.0, T _maxVal = 1.0)
    {
        if (trans)
            _data.transpose();
        int rows = _data.rows();
        int cols = _data.columns();

        for (int i = 0; i < rows; i++)
        {
            int _start = i * cols;
            int _end = _start + cols;


            auto _begin = _data.begin() + _start;
            auto _end_it = _begin + _end;

            std::transform(_begin, _end_it, _begin, [&mnMax, i, _minVal, _maxVal](const T& x) {
                return  mnMax.at(i, 0) + 
                        (mnMax.at(i, 1) - mnMax.at(i, 0)) * (x - _minVal) / 
                        (_maxVal - _minVal); });
        }
        if (trans)
            _data.transpose();
    }

    template<typename T>
    void orthonorm(MatArray<T>& columnVectors)
    {
        int cols = columnVectors.columns();
        int rows = columnVectors.rows();

        auto orthoProj = [](int uPos, int vPos, const MatArray<T>& UVmat) ->MatArray<T> {
            int uvCols = UVmat.columns();
            MatArray<T> retMat = zeros<T>(1, uvCols);
            
            auto u_begin = UVmat.begin() + uvCols * uPos;
            auto v_begin = UVmat.begin() + uvCols * vPos;
            auto u_end = u_begin + uvCols;

            T uDotv = dotProduct(UVmat, uPos, vPos, false);
            T uDotu = dotProduct(UVmat, uPos, uPos, false);
            
            T scl = uDotv / uDotu;
            
            std::transform(u_begin, u_end, retMat.begin(), [scl](const T& x) {return x * scl; });
            return retMat;
        };

        MatArray<T> Es = columnVectors.getTransposed();
        MatArray<T> Us = columnVectors.getTransposed();

        MatArray<T>Projs = zeros<T>(1, rows);
        auto matNorm = getEuclideanNorm<T>(columnVectors, 0, true);
        for (int i = 0; i < rows; i++)
        {
            Es[i] /= matNorm;
        }

        for (int i = 1; i < cols; i++)
        {
            for (int k=0; k < i; k++)
            {
                Projs += orthoProj(k, i, Us);

            }
            auto u_begin = Us.begin() + rows * i;
            auto u_end = u_begin + rows;
            std::transform(u_begin, u_end, Projs.begin(), u_begin, 
                [](const T& x, const T& y) {return x - y; });
            T uiNorm = getEuclideanNorm<T>(Us, i, false);

            for (int j = 0; j < rows; j++)
            {
                Es.at(i, j) = Us.at(i, j)/uiNorm;
            }

            Projs.resetToZeros();
        }

        Es.transpose();
        std::copy(Es.begin(), Es.end(), columnVectors.begin());

    }

    MatArray<double> inv(const MatArray<double>& arg);
    MatArray<float> inv(const MatArray<float>& arg);
    MatArray<double> pinv(const MatArray<double>& arg);
    MatArray<float> pinv(const MatArray<float>& arg);

    MatArray<double> chol(const MatArray<double>& mat, char uplo = 'U');
    MatArray<double> inv_up(const MatArray<double>& mat);
    void eig(MatArray<double>& matA, MatArray<double>& matLambda, char uplo='U');
    

    template<typename T>
    void lobpcg_ortho(MatArray<T>& lhs, double tol = 1e-6)
    {
        MatArray<T> lhs_copy = lhs;
        int cols = lhs.columns();
        int rows = lhs.rows();
        MatArray<T> idt = getIdentityMat<T>(cols);

        auto convergence = [&idt, tol](const MatArray<T>& X) -> bool {
            return getMatNorm<T>((X.getTransposed() & X) - idt) < tol;
        };

        T shift{ 0 };

        lhs.print();

        while (!convergence(lhs_copy))
        {
            MatArray<T> M = lhs_copy.getTransposed() & lhs_copy;
            for (int i = 0; i < cols; i++)
            {
                M.at(i, i) += shift;
            }
            try
            {
                M = chol(M, 'U');

                lhs_copy = lhs_copy & inv_up(M);
                shift = T(0);
            }
            catch (const std::exception&)
            {
                shift += T(100) * getMatNorm(lhs_copy) * 1.0 / 1e-10;
                M.print();
                exit(-1);
            }
        }

        std::copy(lhs_copy.begin(), lhs_copy.end(), lhs.begin());
    }

    template<typename T>
    MatArray<T> concatColMat(const MatArray<T>& lhs, const MatArray<T>& rhs)
    {
        int rows = lhs.rows();
        int lhs_cols = lhs.columns();
        int rhs_cols = rhs.columns();
        int cCols = lhs_cols + rhs_cols;

        MatArray<T> retMat = zeros<T>(rows, cCols);
        for (int i = 0; i < rows; i++)
        {
            int colPos = 0;
            for (int j = 0; j < lhs_cols; j++)
            {
                retMat.at(i, colPos) = lhs.at(i, j);
                colPos++;
            }

            for (int j = 0; j < rhs_cols; j++)
            {
                retMat.at(i, colPos) = rhs.at(i, j);
                colPos++;
            }
        }
        return retMat;
    }

    template<typename T>
    MatArray<T> concatColMat(const MatArray<T>& lhs, const MatArray<T>& rhs, const MatArray<T>& nhs)
    {
        int rows = lhs.rows();
        int lhs_cols = lhs.columns();
        int rhs_cols = rhs.columns();
        int nhs_cols = nhs.columns();
        int cCols = lhs_cols + rhs_cols + nhs_cols;

        MatArray<T> retMat = zeros<T>(rows, cCols);
        for (int i = 0; i < rows; i++)
        {
            int colPos = 0;
            for (int j = 0; j < lhs_cols; j++)
            {
                retMat.at(i, colPos) = lhs.at(i, j);
                colPos++;
            }

            for (int j = 0; j < rhs_cols; j++)
            {
                retMat.at(i, colPos) = rhs.at(i, j);
                colPos++;
            }

            for (int j = 0; j < nhs_cols; j++)
            {
                retMat.at(i, colPos) = nhs.at(i, j);
                colPos++;
            }
        }
        return retMat;
    }

    template<typename T>
    void lobpcg_ortho(MatArray<T>& lhs, const MatArray<T>& rhs, double tol = 1e-6)
    {
        MatArray<T> Y_t = rhs.getTransposed();
        MatArray<T>lhs_copy = lhs;
        auto convergence = [tol](const MatArray<T>& Y_t, const MatArray<T>& X) -> bool {
            return getMatNorm<T>(Y_t & X) < tol;
        };

        while(!convergence(Y_t, lhs_copy))
        {
            lhs_copy = lhs_copy - ((rhs & Y_t) & lhs_copy);
            lobpcg_ortho(lhs_copy, tol);
        }

        std::copy(lhs_copy.begin(), lhs_copy.end(), lhs.begin());
    }
}

