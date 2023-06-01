#pragma  once
#include "Cell.hpp"

class GoAndFreeParking : public Cell
{
public:
    /*
     * Public constructor for deserialization and creation
     */
    GoAndFreeParking(const int ID, const string NAME, vector<std::string> playerIDsOnCell);

    /**
     * @brief Empty call back function when a player lands on a Go or Free Parking cell.
     * It inherits Cell class. This class is for the instantiation of these two cells only.
     * No data member and member functions are necessary.
     */
    CellActivations activate();
};
