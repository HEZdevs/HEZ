#include "vector.h"

namespace hez {
	/// константы
	const vector vector::zero = vector(0.0f, 0.0f, 0.0f);
	const vector vector::unitX = vector(1.0f, 0.0f, 0.0f);
	const vector vector::unitY = vector(0.0f, 1.0f, 0.0f);
	const vector vector::unitZ = vector(0.0f, 0.0f, 1.0f);
}
