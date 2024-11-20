#include "MatArray.h"






namespace nims_n {
	extern "C" {
		void dgetri_(int* N, double* A, int* lda, int* IPIV, double* WORK, int* lwork, int* INFO);
		void dgetrf_(int* M, int* N, double* A, int* lda, int* IPIV, int* INFO);
		void sgetri_(int* N, float* A, int* lda, int* IPIV, float* WORK, int* lwork, int* INFO);
		void sgetrf_(int* M, int* N, float* A, int* lda, int* IPIV, int* INFO);
		void dpotrf_(char* uplo, int* n, double* A, int* ida, int* INFO);
		void dsyev_(char* jobz, char* uplo, int* N, double* A, int* LDA, double* W, double* WORK, int* LWORK, int* INFO);
	}

    MatArray<double> inv(const MatArray<double>& arg)
    {

		int m = arg.rows();
		int n = arg.columns();
		int lda = n;
		int lwork = n * m;
		std::vector<double> ret_data(lwork);
		std::copy(std::execution::par_unseq, arg.begin(), arg.end(), ret_data.begin());
		std::vector<double> work(lwork, 0.0);
		std::vector<int> ipv(n);

		int info{ 1 };
		double* A = ret_data.data();
		dgetrf_(&m, &n, A, &lda, ipv.data(), &info);

		if (info == 0)
			dgetri_(&n, A, &lda, ipv.data(), work.data(), &lwork, &info);
		else {
			std::stringstream matPrint;
			arg.print(matPrint);
			throw std::runtime_error("Non invertible matraix: \n" + matPrint.str());
		}

        return MatArray<double>(std::move(ret_data), m, n);
    }

	MatArray<float> inv(const MatArray<float>& arg)
	{

		int m = arg.rows();
		int n = arg.columns();
		int lda = n;
		int lwork = n * m;
		std::vector<float> ret_data(lwork);
		std::copy(std::execution::par_unseq, arg.begin(), arg.end(), ret_data.begin());
		std::vector<int> ipv(n);
		std::vector<float> work(lwork, 0.0f);

		int info{ 1 };
		float* A = ret_data.data();
		sgetrf_(&m, &n, A, &lda, ipv.data(), &info);

		if (info == 0)
			sgetri_(&n, A, &lda, ipv.data(), work.data(), &lwork, &info);
		else {
			std::stringstream matPrint;
			arg.print(matPrint);
			throw std::runtime_error("Non invertible matraix: \n" + matPrint.str());
		}

		return MatArray<float>(std::move(ret_data), m, n);
	}

    MatArray<double> pinv(const MatArray<double>& arg)
    {
		if (arg.columns() == 1 || arg.rows() == 1)
			return 1.0 / arg;
		auto arg_t = arg.getTransposed();

        return inv(arg_t & arg) & arg_t;
    }

	MatArray<float> pinv(const MatArray<float>& arg)
	{
		if (arg.columns() == 1 || arg.rows() == 1)
			return 1.0f / arg;
		auto arg_t = arg.getTransposed();

		return inv(arg_t & arg) & arg_t;
	}
	MatArray<double> chol(const MatArray<double>& mat, char uplo)
	{
		int n = mat.columns();
		std::vector<double> ret_data(n * n);
		if (n == 1)
		{
			ret_data[0] = std::sqrt(mat[0]);
		}
		else {
			int lda = n;
			int info = 0;
			std::copy(mat.begin(), mat.end(), ret_data.begin());
			double* A = ret_data.data();
			dpotrf_(&uplo, &n, A, &lda, &info);

			if (info != 0)
			{
				std::cout << info << "\n";
				std::stringstream matPrint;
				mat.print(matPrint);
				throw std::runtime_error("Failed factorization: \n" + matPrint.str());
			}

			for (int i{ 0 }; i < n; i++)
				for (int j{ 0 }; j < n; j++)
				{
					if (uplo == 'U')
					{
						if (j > i)
						{
							int idx = i * n + j;
							ret_data[idx] = 0.0;
						}
					}
					else
					{
						if (j < i)
						{
							int idx = i * n + j;
							ret_data[idx] = 0.0;
						}
					}

				}
		}
		

		return MatArray<double>(std::move(ret_data), n, n);
	}
	MatArray<double> inv_up(const MatArray<double>& mat)
	{
		int n = mat.columns();
		int jc = n;
		int jd = jc + 1;
		std::vector<double> ret_data(n * n);
		std::copy(mat.begin(), mat.end(), ret_data.begin());
		std::vector<double> diag(n);
		
		double* pii = ret_data.begin()._Ptr;
		double* pd = diag.begin()._Ptr;
		double v = 0.0;
		for (int i = 0; i < n; i++, pii += jd, pd++) {
			v = *pd = *pii;
			*pii = 1.0 / v;
		}

		pii -= jd;
		double* pin = pii - 1;
		pii -= jd;
		pd -= 2;

		for (int i = n - 1; i > 0; i--, pii -= jd, pin--, pd--) {
			double* pij = pin;
			for (int j = n; j > i; j--, pij -= jc) {
				v = 0;
				double* pik = pii + jc;
				double* pkj = pij + 1;
				for (int k = i + 1; k <= j; k++, pik += jc, pkj++) {
					v -= (*pik) * (*pkj);
				}
				*pij = v / (*pd);
			}
		}

		return MatArray<double>(std::move(ret_data), n, n);
	}

	void eig(MatArray<double>& matA, MatArray<double>& matLambda, char uplo)
	{
		char jobz = 'V';
		int n = matA.columns();
		int lda = matA.rows();
		int lwork = 3 * n - 1;
		int info = 0;
		std::vector<double> work(lwork);
		double* lamda = matLambda.begin()._Ptr;
		double* A = matA.begin()._Ptr;

		dsyev_(&jobz, &uplo, &n, A, &lda, lamda, work.data(), &lwork, &info);

		if (info != 0)
		{
			std::stringstream matPrint;
			matA.print(matPrint);
			throw std::runtime_error("Failed eigen solution: \n" + matPrint.str());
		}
	}
}