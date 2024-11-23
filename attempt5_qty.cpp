#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

// Structure to hold product dimensions
struct ProductDimensions {
    double length;
    double breadth;
    double height;
};

// Structure to hold packing results
struct PackingResult {
    int cartonIndex;
    int orientation;
    int fitLengthwise;
    int fitBreadthwise;
    int fitHeightwise;
    double cartonWeight;
    int numCartons;
    vector<pair<int, int>> cartonDistribution;
};

// Global constants
const double cm_to_inch = 0.393701;
const double ft_to_inch = 12.0;
const double g_to_kg = 0.001;
const double pounds_to_kg = 0.453592;

// Global inventory array with available quantities
const int inventory[5][5] = {
    {10, 12, 14, 16, 18},    // Lengths
    {8, 10, 12, 14, 16},     // Breadths
    {6, 8, 10, 12, 14},      // Heights
    {30, 38, 46, 60, 76},    // Weight limits in kg
    {50, 40, 30, 20, 10}     // Available quantities of each carton
};

bool allCartonsUsed(const vector<bool>& used) {
    return all_of(used.begin(), used.end(), [](bool b) { return b; });
}

double getWeight() {
    cout << "Enter the unit of measurement for the weight of each product: " << endl;
    cout << "1. kg \n2. g \n3. pounds" << endl;
    
    int weightUnitChoice;
    cin >> weightUnitChoice;
    
    double weightPerProduct;
    cout << "Enter the weight of each product: ";
    cin >> weightPerProduct;

    // Convert to kg
    switch (weightUnitChoice) {
        case 2: // g to kg
            weightPerProduct *= g_to_kg;
            break;
        case 3: // pounds to kg
            weightPerProduct *= pounds_to_kg;
            break;
    }

    return weightPerProduct;
}

ProductDimensions getDimensions() {
    ProductDimensions dims = {0, 0, 0};
    double radius = 0;
    
    cout << "What is Shape of the product: " << endl;
    cout << "1. Cuboid \n2. Cylinder \n3. Sphere \n4. Cube" << endl;
    int choice;
    cin >> choice;

    cout << "Enter the unit of measurement: " << endl;
    cout << "1. cm \n2. inch \n3. ft" << endl;
    int unitChoice;
    cin >> unitChoice;

    cout << "Enter Dimensions of the Product to be Shipped: " << endl;

    switch (choice) {
        case 1:
            cout << "Enter the Length: ";
            cin >> dims.length;
            cout << "Enter the Breadth: ";
            cin >> dims.breadth;
            cout << "Enter the Height: ";
            cin >> dims.height;
            break;
        case 4:
            cout << "Enter the Length: ";
            cin >> dims.length;
            dims.breadth = dims.length;
            dims.height = dims.length;
            break;
        case 2:
            cout << "Enter the Height: ";
            cin >> dims.height;
            cout << "Enter the Radius: ";
            cin >> radius;
            dims.length = radius * 2;
            dims.breadth = radius * 2;
            break;
        case 3:
            cout << "Enter the Radius: ";
            cin >> radius;
            dims.length = radius * 2;
            dims.breadth = radius * 2;
            dims.height = radius * 2;
            break;
    }

    switch (unitChoice) {
        case 1: // cm to inch
            dims.length *= cm_to_inch;
            dims.breadth *= cm_to_inch;
            dims.height *= cm_to_inch;
            break;
        case 3: // ft to inch
            dims.length *= ft_to_inch;
            dims.breadth *= ft_to_inch;
            dims.height *= ft_to_inch;
            break;
    }

    return dims;
}

PackingResult calculateOptimalPacking(const ProductDimensions& dims, double weightPerProduct, int quantity) {
    PackingResult result;
    result.numCartons = numeric_limits<int>::max();
    result.cartonIndex = -1;
    result.orientation = 0;
    result.fitLengthwise = 0;
    result.fitBreadthwise = 0;
    result.fitHeightwise = 0;
    result.cartonWeight = 0;

    int remainingQuantity = quantity;
    vector<bool> cartonUsed(5, false);

    while (remainingQuantity > 0 && !allCartonsUsed(cartonUsed)) {
        int bestCartonIndex = -1;
        int bestOrientation = 0;
        int bestProductsPacked = 0;
        double bestWeight = 0;
        int bestFitL = 0, bestFitB = 0, bestFitH = 0;
        int minCartonsNeeded = numeric_limits<int>::max();

        for (int i = 0; i < 5; ++i) {
            if (cartonUsed[i] || inventory[4][i] <= 0) continue;

            for (int o = 1; o <= 3; ++o) {
                int num_boxes_lengthwise = 0, num_boxes_breadthwise = 0, num_boxes_heightwise = 0;

                if (o == 1) {
                    num_boxes_lengthwise = (inventory[0][i] > dims.length + 2) ? (inventory[0][i] - 2) / dims.length : 0;
                    num_boxes_breadthwise = (inventory[1][i] > dims.breadth + 2) ? (inventory[1][i] - 2) / dims.breadth : 0;
                    num_boxes_heightwise = (inventory[2][i] > dims.height + 2) ? (inventory[2][i] - 2) / dims.height : 0;
                } else if (o == 2) {
                    num_boxes_lengthwise = (inventory[0][i] > dims.breadth + 2) ? (inventory[0][i] - 2) / dims.breadth : 0;
                    num_boxes_breadthwise = (inventory[1][i] > dims.length + 2) ? (inventory[1][i] - 2) / dims.length : 0;
                    num_boxes_heightwise = (inventory[2][i] > dims.height + 2) ? (inventory[2][i] - 2) / dims.height : 0;
                } else {
                    num_boxes_lengthwise = (inventory[0][i] > dims.height + 2) ? (inventory[0][i] - 2) / dims.height : 0;
                    num_boxes_breadthwise = (inventory[1][i] > dims.length + 2) ? (inventory[1][i] - 2) / dims.length : 0;
                    num_boxes_heightwise = (inventory[2][i] > dims.breadth + 2) ? (inventory[2][i] - 2) / dims.breadth : 0;
                }

                if (num_boxes_lengthwise == 0 || num_boxes_breadthwise == 0 || num_boxes_heightwise == 0) {
                    continue;
                }

                int max_products_packed = num_boxes_lengthwise * num_boxes_breadthwise * num_boxes_heightwise;
                double totalWeight = weightPerProduct * max_products_packed;

                while (totalWeight > inventory[3][i]) {
                    max_products_packed--;
                    totalWeight = weightPerProduct * max_products_packed;
                }

                if (max_products_packed > 0) {
                    int cartonsNeeded = min(
                        (remainingQuantity + max_products_packed - 1) / max_products_packed,
                        inventory[4][i]
                    );

                    if (cartonsNeeded < minCartonsNeeded) {
                        minCartonsNeeded = cartonsNeeded;
                        bestCartonIndex = i;
                        bestOrientation = o;
                        bestProductsPacked = max_products_packed;
                        bestWeight = totalWeight;
                        bestFitL = num_boxes_lengthwise;
                        bestFitB = num_boxes_breadthwise;
                        bestFitH = num_boxes_heightwise;
                    }
                }
            }
        }

        if (bestCartonIndex == -1) {
            cartonUsed[result.cartonIndex] = true;
            continue;
        }

        int productsHandled = min(remainingQuantity, 
                                bestProductsPacked * inventory[4][bestCartonIndex]);
        int cartonsUsed = (productsHandled + bestProductsPacked - 1) / bestProductsPacked;

        result.cartonDistribution.push_back({bestCartonIndex, cartonsUsed});
        remainingQuantity -= (cartonsUsed * bestProductsPacked);

        cartonUsed[bestCartonIndex] = true;

        if (result.cartonIndex == -1) {
            result.cartonIndex = bestCartonIndex;
            result.orientation = bestOrientation;
            result.fitLengthwise = bestFitL;
            result.fitBreadthwise = bestFitB;
            result.fitHeightwise = bestFitH;
            result.cartonWeight = bestWeight;
        }
    }

    if (remainingQuantity > 0) {
        result.numCartons = numeric_limits<int>::max();
        return result;
    }

    result.numCartons = 0;
    for (const auto& dist : result.cartonDistribution) {
        result.numCartons += dist.second;
    }

    return result;
}

void displayResults(const PackingResult& result) {
    cout << "Optimal Packing Strategy:" << endl;
    cout << "Best carton size: " 
         << inventory[0][result.cartonIndex] << " x "
         << inventory[1][result.cartonIndex] << " x "
         << inventory[2][result.cartonIndex] << " inches" << endl;
    
    cout << "Best orientation for packing: ";
    if (result.orientation == 1) cout << "Length-wise" << endl;
    else if (result.orientation == 2) cout << "Breadth-wise" << endl;
    else cout << "Height-wise" << endl;

    cout << "Packing arrangement:" << endl;
    cout << "Products per length of carton: " << result.fitLengthwise << endl;
    cout << "Products per breadth of carton: " << result.fitBreadthwise << endl;
    cout << "Number of stacks: " << result.fitHeightwise << endl;
    cout << "Products per carton: " << result.fitLengthwise * result.fitBreadthwise * result.fitHeightwise << endl;
    cout << "Weight of each packed carton: " << result.cartonWeight << " kg" << endl;
    cout << "Carton weight limit: " << inventory[3][result.cartonIndex] << " kg" << endl;
    cout << "Number of cartons required: " << result.numCartons << endl;

    cout << "\nCarton Distribution:" << endl;
    for (const auto& dist : result.cartonDistribution) {
        cout << "Using " << dist.second << " cartons of size "
             << inventory[0][dist.first] << "x"
             << inventory[1][dist.first] << "x"
             << inventory[2][dist.first] << " inches"
             << " (Available: " << inventory[4][dist.first] << ")" << endl;
    }
    
    cout << "All products have been packed within weight and dimension limits, with 1 inch left for packing material." << endl;
}

int main() {
    cout << "Box Size Detector" << endl;
    
    ProductDimensions dims = getDimensions();
    if (dims.length <= 0 || dims.breadth <= 0 || dims.height <= 0) {
        cout << "Invalid dimensions provided. Please enter positive values." << endl;
        return 1;
    }

    double weightPerProduct = getWeight();
    if (weightPerProduct <= 0) {
        cout << "Invalid weight provided. Please enter positive values." << endl;
        return 1;
    }
    
    cout << "\nEnter the Number of products to be Shipped: ";
    int quantity;
    cin >> quantity;
    
    if (quantity <= 0) {
        cout << "Invalid quantity. Please enter a positive number." << endl;
        return 1;
    }

    PackingResult result = calculateOptimalPacking(dims, weightPerProduct, quantity);
    
    if (result.numCartons == numeric_limits<int>::max()) {
        cout << "No suitable carton found. Please check the dimensions and try again." << endl;
        return 1;
    }

    displayResults(result);
    return 0;
}