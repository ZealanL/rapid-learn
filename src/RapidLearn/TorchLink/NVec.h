#pragma once
#include "../Lists.h"

namespace RPLC {
	// Quick implementation of an N-dimensional vector for temporary storage
	// Made to be an intermediate step for efficiently converting data to a Python object
	template <typename T>
	class NVec {
	protected:
		std::vector<T> _data;
		std::vector<size_t> _dims;

	public:

		NVec() = default;

		NVec(const T* data, size_t dataAmount, const std::vector<size_t>& dims) {
			_data = std::vector<T>(data, data + dataAmount);
			_dims = dims;
		}

		NVec(const std::vector<T>& data)
			: NVec(data.data(), data.size(), { data.size() }) {}

		NVec(const std::vector<std::vector<T>>& data) {
			size_t dim0 = data.size();
			RG_ASSERT(dim0 > 0);
			size_t dim1 = data[0].size();
			RG_ASSERT(dim1 > 0);

			_data.reserve(dim0 * dim1);

			for (auto& row : data) {
				RG_ASSERT(row.size() == dim1);
				_data.insert(_data.end(), row.begin(), row.end());
			}

			_dims = { dim0, dim1 };
		}

		const std::vector<T>& GetData() const { return _data; }
		std::vector<T>& GetData() { return _data; }

		const std::vector<size_t>& GetDims() const { return _dims; }

		std::vector<size_t> CalcByteStrides() const {
			std::vector<size_t> result = {};
			size_t scale = 1;
			for (size_t dim : _dims) {
				result.push_back(sizeof(T) * scale);
				scale *= dim;
			}
			
			return result;
		}
	};

	typedef NVec<float> NVecF;
	typedef NVec<int> NVecI;
}