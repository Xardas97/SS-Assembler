#include "equSymbol.h"

ostream& operator<<(ostream& out, const EquSymbol& es) {
	return out << es.label << "\t\t\t" << es.value << "\t\t\t";
}
