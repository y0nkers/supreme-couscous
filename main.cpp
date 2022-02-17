#include <iostream>
#include <conio.h>
#include <chrono>
#include <fstream>
#include <random>
#include <windows.h>

#define PLOT 1 // 1 if you want to plot

using std::cout;
using std::cin;
using std::endl;

#define PI 3.14159265358979323846
#define g 9.80665

void material_point() {
    long double IMITATION_MODEL_STEP = 0.001;
    long double initial_speed = 0.0, angle = 0.0;
    float AIR_RESISTANCE_COEFFICIENT = 0; // in range [0;1]
    do {
        cout << "Enter initial speed (m/s): ";
        cin >> initial_speed;
    } while (initial_speed < 0.0);

    do {
        cout << "Enter the angle in degrees [0; 180]: ";
        cin >> angle;
    } while (angle > 180.0 || angle < 0.0);

    do {
        cout << "Enter air resistance coefficient for imitation model [0; 1]: ";
        cin >> AIR_RESISTANCE_COEFFICIENT;
    } while (AIR_RESISTANCE_COEFFICIENT > 1 || AIR_RESISTANCE_COEFFICIENT < 0);

    do {
        cout << "Enter imitation model step [0.00001; 1.0]: ";
        cin >> IMITATION_MODEL_STEP;
    } while (IMITATION_MODEL_STEP > 1.0 || IMITATION_MODEL_STEP < 0.00001);

    cout << endl;
    if (angle < 90.0) cout << "Throw direction: forward\n";
    else if (angle > 90.0) cout << "Throw direction: back\n";
    else cout << "Throw direction: perpendicular up\n";
    angle = angle * PI / 180; // From degrees to radians

    long double analytic_distance = (initial_speed * initial_speed * fabs(sin(2 * angle))) / g;

    // Imitation model
    long double speed_x_last = initial_speed * fabs(cos(angle)), 
                speed_x_curr = 0.0,
                speed_y_last = initial_speed * fabs(sin(angle)),
                speed_y_curr = 0.0;
    long double x_last = 0.0, 
                x_curr = 0.0,
                y_last = 0.0,
                y_curr = 0.0;

#if PLOT == 1
    std::ofstream data; // stream from points in file
    data.open("C:/points.txt");
    if (!data) {
        std::cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
#endif // PLOT == 1

    auto imitation_begin = std::chrono::steady_clock::now();

    do {
        speed_x_curr = speed_x_last - AIR_RESISTANCE_COEFFICIENT * IMITATION_MODEL_STEP * speed_x_last;
        speed_y_curr = speed_y_last - g * IMITATION_MODEL_STEP - AIR_RESISTANCE_COEFFICIENT * IMITATION_MODEL_STEP * speed_y_last;
        x_curr = x_last + speed_x_last * IMITATION_MODEL_STEP;
        y_curr = y_last + speed_y_last * IMITATION_MODEL_STEP;
        if (y_curr >= 0.0) {
            speed_x_last = speed_x_curr;
            speed_y_last = speed_y_curr;
            x_last = x_curr;
            y_last = y_curr;
#if PLOT == 1
            data << x_last << " " << y_last << endl; // write point (x, y) in file
#endif // PLOT == 1
        }
        else break;
    } while (y_curr >= 0.0);

    long double imitation_distance = x_last + (speed_x_last * y_last / speed_y_last);
    auto imitation_end = std::chrono::steady_clock::now();

#if PLOT == 1
    data.close();
    ShellExecuteA(NULL, NULL, "plot.exe", NULL, NULL, SW_SHOWDEFAULT); // execute visualization for imitation model plot
#endif // PLOT == 1

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(imitation_end - imitation_begin);

    cout.precision(10);
    cout << std::fixed;
    cout << "\nANALYTICAL MODEL:\nDistance: " << analytic_distance << " meters.\n";
    cout << "\nIMITATION MODEL:\nStep = " << IMITATION_MODEL_STEP << "\nAir resistance = " << AIR_RESISTANCE_COEFFICIENT << "\nDistance: " << imitation_distance << " meters.\nElapsed time: " << elapsed_time.count() << " ms\n";
}

void getFigureCoords(long double figure_coords[2][2], long double circle_radius, long double half_width, long double half_height) {
    // if circle inside rectangle
    if (circle_radius <= half_width && circle_radius <= half_height) {
        figure_coords[0][0] = -half_width;
        figure_coords[0][1] = half_height;
        figure_coords[1][0] = half_width;
        figure_coords[1][1] = -half_height;
    }
    // if rectangle inside circle
    else if (circle_radius > half_width && circle_radius > half_height) {
        figure_coords[0][0] = -circle_radius;
        figure_coords[0][1] = circle_radius;
        figure_coords[1][0] = circle_radius;
        figure_coords[1][1] = -circle_radius;
    }
    // If the rectangle is wider than the circle, but lower than it
    else if (half_width > circle_radius && half_height < circle_radius) {
        figure_coords[0][0] = -half_width;
        figure_coords[0][1] = circle_radius;
        figure_coords[1][0] = half_width;
        figure_coords[1][1] = -circle_radius;
    }
    // If the rectangle is taller than the circle, but narrower than it
    else if (half_width < circle_radius && half_height > circle_radius) {
        figure_coords[0][0] = -circle_radius;
        figure_coords[0][1] = half_height;
        figure_coords[1][0] = circle_radius;
        figure_coords[1][1] = -half_height;
    }
}

bool isInFigure(long double point_x, long double point_y, long double circle_radius, long double half_width, long double half_height) {
    bool xInRect = point_x >= -half_width && point_x <= half_width;
    bool yInRect = point_y >= -half_height && point_y <= half_height;
    bool inCircle = sqrt(point_x * point_x + point_y * point_y) < circle_radius;
    return xInRect && yInRect && inCircle;
}

void area_figure() {
    srand(time(NULL));
    int imitation_model_points = 0;
    long double circle_radius = 0.0,
                rectangle_width = 0.0,
                rectangle_height = 0.0;

    do {
        cout << "Enter circle radius: ";
        cin >> circle_radius;
    } while (circle_radius < 0.0);

    do {
        cout << "Enter rectangle width: ";
        cin >> rectangle_width;
    } while (rectangle_width < 0.0);

    do {
        cout << "Enter rectangle height: ";
        cin >> rectangle_height;
    } while (rectangle_height < 0.0);

    do {
        cout << "Enter amount of points for imitation model: ";
        cin >> imitation_model_points;
    } while (imitation_model_points < 0);

    // Analytic model
    long double analytic_area = 0.0,
                ABO_area = 0.0,
                ACO_area = 0.0;
    long double half_width = rectangle_width / 2,
                half_height = rectangle_height / 2;

    // if circle inside rectangle
    if (circle_radius <= half_width && circle_radius <= half_height) analytic_area = PI * circle_radius * circle_radius;
    // if rectangle inside circle
    else if (circle_radius > half_width && circle_radius > half_height) analytic_area = rectangle_height * rectangle_width;
    // If the rectangle is wider than the circle, but lower than it
    else if (half_width > circle_radius && half_height < circle_radius) {
        long double cathetus_ab = sqrt((circle_radius * circle_radius) - half_height * half_height); // AB = sqrt(AO^2 - OB^2)
        ABO_area = cathetus_ab * half_height / 2; // 1/2 * AB * OB
        long double OAB_degree = acos(cathetus_ab / circle_radius) / PI * 180;
        ACO_area = PI * circle_radius * circle_radius * OAB_degree / 360;
        analytic_area = 4 * (ABO_area + ACO_area); // S = 4S_aboc = 4(S_abo + S_aoc)
    }
    // If the rectangle is taller than the circle, but narrower than it
    else if (half_width < circle_radius && half_height > circle_radius) {
        long double cathetus_ac = sqrt((circle_radius * circle_radius) - half_width * half_width/ 2);
        ACO_area = cathetus_ac * half_width / 2; // 1/2 * AC * OC
        long double OAC_degree = acos(cathetus_ac / circle_radius) / PI * 180;
        ABO_area = PI * circle_radius * circle_radius * OAC_degree / 360;
        analytic_area = 4 * (ABO_area + ACO_area);
    }

    // Imitation model
    long double figure_coords[2][2]{ {0, 0}, {0, 0} }; // left up, right down (X,Y)
    
#if PLOT == 1
    std::ofstream data; // stream from points in file
    data.open("C:/montecarlo.txt");
    if (!data) {
        std::cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    data << rectangle_width << " " << rectangle_height << " " << circle_radius << endl;
#endif // PLOT == 1

    auto imitation_begin = std::chrono::steady_clock::now();
    getFigureCoords(figure_coords, circle_radius, half_width, half_height); // getting coords for created figure
    long double created_figure_area = figure_coords[1][0] * 2 * figure_coords[0][1] * 2; // figure width * figure height
    long double point_x = 0.0, point_y = 0.0;
    int k = 0; // counter for points that hit the intersection of shapes
    
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> x(figure_coords[0][0], figure_coords[1][0]);
    std::uniform_real_distribution<> y(figure_coords[1][1], figure_coords[0][1]);

    for (int i = 0; i < imitation_model_points; i++) {
        // Creating pseudo random numbers from figure_coords range: 
        point_x = x(gen);
        point_y = y(gen);
#if PLOT == 1
        data << point_x << " " << point_y << endl; // write point (x, y) in file
#endif // PLOT == 1
        if (isInFigure(point_x, point_y, circle_radius, half_width, half_height)) k++;
    }

    long double imitation_area = created_figure_area * k / imitation_model_points;
    auto imitation_end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(imitation_end - imitation_begin);

#if PLOT == 1
    data.close();
    ShellExecuteA(NULL, NULL, "montecarlo2.exe", NULL, NULL, SW_SHOWDEFAULT); // execute visualization for monte-carlo
#endif // PLOT == 1

    cout.precision(10);
    cout << std::fixed;
    cout << "\nANALYTICAL MODEL:\nArea: " << analytic_area << " c.u.\n";
    cout << "\nIMITATION MODEL:\nTest points: " << k << " of " << imitation_model_points << "\nArea: " << imitation_area << " c.u.\nElapsed time: " << elapsed_time.count() << " ms\n";;
}

int main() {
    system("chcp 1251");

    bool menu = true;
    int input;
    while (menu) {
        cout << "\n1. Calculate distance of material point.\n2. Calculate area of figure.\n3. Exit.\n\nYour choice: ";
        cin >> input;
        switch (input) {
        case 1:
            material_point();
            break;
        case 2:
            area_figure();
            break;
        case 3:
            menu = false;
            break;
        default:
            cout << "u sure?\n";
            menu = false;
            break;
        }
    }

    system("PAUSE");
    return 0;
}