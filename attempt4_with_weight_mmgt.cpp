#include <iostream>  // Include the iostream library for input/output
#include <limits>    // Include the limits library to use numeric limits
#include <algorithm> // Include the algorithm library for potential future use
using namespace std;

int main() {
    // Print the initial message
    cout << "Box Size Detector" << endl;
    
    // Ask the user for the shape of the product
    cout << "What is Shape of the product: " << endl;
    cout << "1. Cuboid \n2. Cylinder \n3. Sphere \n4. Cube" << endl;
    
    int choice;
    cin >> choice;

    // Ask the user for the unit of measurement
    cout << "Enter the unit of measurement: " << endl;
    cout << "1. cm \n2. inch \n3. ft" << endl;
    
    int unitChoice;
    cin >> unitChoice;
    
    // Conversion factors
    const double cm_to_inch = 0.393701;
    const double ft_to_inch = 12.0;
    
    double len = 0, br = 0, ht = 0, rd = 0;

    cout << "Enter Dimensions of the Product to be Shipped: " << endl;

    switch (choice) {
        case 1:
            cout << "Enter the Length: ";
            cin >> len;
            cout << "Enter the Breadth: ";
            cin >> br;
            cout << "Enter the Height: ";
            cin >> ht;
            break;
        case 4:
            cout << "Enter the Length: ";
            cin >> len;
            br = len;
            ht = len;
            break;
        case 2:
            cout << "Enter the Height: ";
            cin >> ht;
            cout << "Enter the Radius: ";
            cin >> rd;
            len = rd * 2;
            br = rd * 2;
            break;
        case 3:
            cout << "Enter the Radius: ";
            cin >> rd;
            len = rd * 2;
            br = rd * 2;
            ht = rd * 2;
            break;
    }

    switch (unitChoice) {
        case 1:
            len *= cm_to_inch;
            br *= cm_to_inch;
            ht *= cm_to_inch;
            break;
        case 3:
            len *= ft_to_inch;
            br *= ft_to_inch;
            ht *= ft_to_inch;
            break;
        case 2:
            break;
        default:
            cout << "Invalid unit choice. Please select 1, 2, or 3." << endl;
            return 1;
    }

    if (len <= 0 || br <= 0 || ht <= 0) {
        cout << "Invalid dimensions provided. Please enter positive values." << endl;
        return 1;
    }

    cout << "\nEnter the Number of products to be Shipped: ";
    int quant;
    cin >> quant;
    
    if (quant <= 0) {
        cout << "Invalid quantity. Please enter a positive number." << endl;
        return 1;
    }

    // New input for weight of each product
    cout << "Enter the unit of measurement for the weight of each product: " << endl;
    cout << "1. kg \n2. g \n3. pounds" << endl;
    
    int weightUnitChoice;
    cin >> weightUnitChoice;
    
    double weightPerProduct;
    cout << "Enter the weight of each product: ";
    cin >> weightPerProduct;

    // Conversion to kg
    const double g_to_kg = 0.001;
    const double pounds_to_kg = 0.453592;
    
    switch (weightUnitChoice) {
        case 1:
            break;
        case 2:
            weightPerProduct *= g_to_kg;
            break;
        case 3:
            weightPerProduct *= pounds_to_kg;
            break;
        default:
            cout << "Invalid weight unit choice. Please select 1, 2, or 3." << endl;
            return 1;
    }

    // Inventory with added weight limits
    int inventory[4][5] = {
        {10, 12, 14, 16, 18}, // Lengths
        {8, 10, 12, 14, 16},  // Breadths
        {6, 8, 10, 12, 14},   // Heights
        {30, 38, 46, 60, 76}  // Weight limits in kg
    };

    int min_outer_cartons = numeric_limits<int>::max();
    int best_carton_index = -1;
    int best_orientation = 0;
    int best_fit_lengthwise = 0, best_fit_breadthwise = 0, best_fit_heightwise = 0;
    double best_carton_weight = 0;

    for (int i = 0; i < 5; ++i) {
        for (int o = 1; o <= 3; ++o) {
            int num_boxes_lengthwise = 0, num_boxes_breadthwise = 0, num_boxes_heightwise = 0;

            if (o == 1) {
                num_boxes_lengthwise = (inventory[0][i] > len + 2) ? (inventory[0][i] - 2) / len : 0;
                num_boxes_breadthwise = (inventory[1][i] > br + 2) ? (inventory[1][i] - 2) / br : 0;
                num_boxes_heightwise = (inventory[2][i] > ht + 2) ? (inventory[2][i] - 2) / ht : 0;
            } else if (o == 2) {
                num_boxes_lengthwise = (inventory[0][i] > br + 2) ? (inventory[0][i] - 2) / br : 0;
                num_boxes_breadthwise = (inventory[1][i] > len + 2) ? (inventory[1][i] - 2) / len : 0;
                num_boxes_heightwise = (inventory[2][i] > ht + 2) ? (inventory[2][i] - 2) / ht : 0;
            } else if (o == 3) {
                num_boxes_lengthwise = (inventory[0][i] > ht + 2) ? (inventory[0][i] - 2) / ht : 0;
                num_boxes_breadthwise = (inventory[1][i] > len + 2) ? (inventory[1][i] - 2) / len : 0;
                num_boxes_heightwise = (inventory[2][i] > br + 2) ? (inventory[2][i] - 2) / br : 0;
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

            int needed_cartons = (quant + max_products_packed - 1) / max_products_packed;
            if (needed_cartons < min_outer_cartons && max_products_packed > 0) {
                min_outer_cartons = needed_cartons;
                best_carton_index = i;
                best_orientation = o;
                best_fit_lengthwise = num_boxes_lengthwise;
                best_fit_breadthwise = num_boxes_breadthwise;
                best_fit_heightwise = num_boxes_heightwise;
                best_carton_weight = totalWeight;
            }
        }
    }

    if (min_outer_cartons == numeric_limits<int>::max()) {
        cout << "No suitable carton found. Please check the dimensions and try again." << endl;
        return 1;
    }

     cout << "Optimal Packing Strategy:" << endl;
    cout << "Best carton size: " 
         << inventory[0][best_carton_index] << " x "
         << inventory[1][best_carton_index] << " x "
         << inventory[2][best_carton_index] << " inches" << endl;
    cout << "Best orientation for packing: ";
    if (best_orientation == 1) cout << "Length-wise" << endl;
    else if (best_orientation == 2) cout << "Breadth-wise" << endl;
    else cout << "Height-wise" << endl;

    cout << "Packing arrangement:" << endl;
    cout << "Products per length of carton: " << best_fit_lengthwise << endl;
    cout << "Products per breadth of carton: " << best_fit_breadthwise << endl;
    cout << "Number of stacks: " << best_fit_heightwise << endl;
    cout << "Products per carton: " << best_fit_lengthwise * best_fit_breadthwise * best_fit_heightwise << endl;
    cout << "Weight of each packed carton: " << best_carton_weight << " kg" << endl;
    cout << "Carton weight limit: " << inventory[3][best_carton_index] << " kg" << endl;
    cout << "Number of cartons required: " << min_outer_cartons << endl;
    cout << "All products have been packed within weight and dimension limits, with 1 inch left for packing material." << endl;

    return 0;
}