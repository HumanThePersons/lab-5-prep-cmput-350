#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 60;

constexpr float PI = 3.14159265358979;  //

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======

        // Function 1 : Ease in out cubic
        std::function<float(float, float, float)> easeInOutCubic = [](float a, float b, float t) -> float {
            // easeInOutCubic from https://easings.net/
            // return x < 0.5 ? 4 * x * x * x : 1 - Math.pow(-2 * x + 2, 3) / 2;
            if (t < 0.5) {
                t  = 4 * std::pow(t, 3);
            }
            else {
                t = 1 - ((std::pow(-2*t + 2, 3)) / 2);
            }
            return (1 - t) * a + t * b;
        };

        // Function 2: Ease out sine
        std::function<float(float, float, float)> easeOutSine = [](float a, float b, float t) -> float {
            // easeOutSine from https://easings.net/
            // return Math.sin((x * Math.PI) / 2)
            t = std::sin((t*PI / 2));
            return (1 - t) * a + t * b;
        };

        // Function 3: Ease in quart
        std::function<float(float, float, float)> easeInQuart = [](float a, float b, float t) -> float {
            // easeInQuart from https://easings.net/
            // return x * x * x * x;
            t = std::pow(t, 4);
            return (1 - t) * a + t * b;
        };

        // Function 4: Ease out quad
        std::function<float(float, float, float)> easeOutQuad = [](float a, float b, float t) -> float {
            // easeOutQuad from https://easings.net/
            // return 1 - (1 - x) * (1 - x);
            t = 1 - (std::pow(1 - t, 2));
            return (1 - t) * a + t * b;
        };

        // Function 5: Ease in out exponent
        std::function<float(float, float, float)> easeInOutExpo = [](float a, float b, float t) -> float {
            // easeInOutExpo from https://easings.net/
            /* return x === 0 ? 0
            : x === 1 ? 1
            : x < 0.5 ? Math.pow(2, 20 * x - 10) / 2
            : (2 - Math.pow(2, -20 * x + 10)) / 2;
            */
            if (t != 0 && t != 1) {  // If t == 0 or t == 1 it doesnt need to be modified
                if (t < 0.5) {
                    t = std::pow(2, (20 * t) - 10) / 2;
                }
                else {
                    t = (2 - std::pow(2, (-20 * t) + 10)) / 2;
                }    
            }
            return (1 - t) * a + t * b;
        };

        // Function 6: Ease in circle
        std::function<float(float, float, float)> easeInCirc = [](float a, float b, float t) -> float {
            // easeInCirc from https://easings.net/
            // return 1 - Math.sqrt(1 - Math.pow(x, 2));
            t = 1 - std::sqrt(1 - std::pow(t, 2));
            return (1 - t) * a + t * b;
        };

        // Function 7: Ease in elastic
        std::function<float(float, float, float)> easeInElastic = [](float a, float b, float t) -> float {
            // easeInElastic from https://easings.net/
            /*
            const c4 = (2 * Math.PI) / 3;
            return x === 0 ? 0
            : x === 1 ? 1
            : -Math.pow(2, 10 * x - 10) * Math.sin((x * 10 - 10.75) * c4);
            */
            const float constant = (2*PI) / 3;
            // Cases where t == 0 and t == 1 have t = 0 and t = 1 implicitly
            if (t != 0 && t != 1){
                t = -1 * (std::pow(2, (10 * t) - 10)) * (std::sin(((t * 10) - 10.75) * constant));
            }
            return (1 - t) * a + t * b;
        };

        // Function 8: Ease out bounce
        std::function<float(float, float, float)> easeOutBounce = [](float a, float b, float t) -> float {
            // easeOutBounce from https://easings.net/
            /*
            const n1 = 7.5625;
            const d1 = 2.75;

            if (x < 1 / d1) {
                return n1 * x * x;
            } else if (x < 2 / d1) {
                return n1 * (x -= 1.5 / d1) * x + 0.75;
            } else if (x < 2.5 / d1) {
                return n1 * (x -= 2.25 / d1) * x + 0.9375;
            } else {
                return n1 * (x -= 2.625 / d1) * x + 0.984375;
            }
            */
            const float n1 = 7.5625;
            const float d1 = 2.75;

            if (t < (1 / d1)) {
                t = n1 * std::pow(t, 2);
            }
            else if (t < (2 / d1)) {
                t = (n1 * (t -= (1.5 / d1)) * t) + 0.75;
            }
            else if (t < (2.5 / d1)) {
                t = (n1 * (t -= (2.25 / d1)) * t) + 0.9375;
            }
            else {
                t = (n1 * (t -= (2.625 / d1)) * t) + 0.984375;
            }
            return (1 - t) * a + t * b;
        };

        // Function in out back
        std::function<float(float, float, float)> easeInOutBack = [](float a, float b, float t) {
            // easeInOutBack from https://easings.net/
            /*
            const c1 = 1.70158;
            const c2 = c1 * 1.525;

            return x < 0.5
            ? (Math.pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
            : (Math.pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
            */
            const float c1 = 1.70158;
            const float c2 = c1 * 1.525;

            if (t < 0.5) {
                t = (std::pow(2 * t, 2) * (((c2 + 1) * 2 * t) - c2)) / 2;
            }
            else {
                t = ((std::pow((2 * t) - 2, 2) * ((c2 + 1) * ((t * 2) - 2) + c2)) + 2) / 2;
            }
            return (1 - t) * a + t * b;
        };

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            switch (key->scancode) {
                case sf::Keyboard::Scancode::Num1:
                    tween = easeInOutCubic;
                    break;

                case sf::Keyboard::Scancode::Num2:
                    tween = easeOutSine;
                    break;

                case sf::Keyboard::Scancode::Num3:
                    tween = easeInQuart;
                    break;

                case sf::Keyboard::Scancode::Num4:
                    tween = easeOutQuad;
                    break;

                case sf::Keyboard::Scancode::Num5:
                    tween = easeInOutExpo;
                    break;

                case sf::Keyboard::Scancode::Num6:
                    tween = easeInCirc;
                    break;

                case sf::Keyboard::Scancode::Num7:
                    tween = easeInElastic;
                    break;

                case sf::Keyboard::Scancode::Num8:
                    tween = easeOutBounce;    
                    break;

                case sf::Keyboard::Scancode::Num9:
                    tween = easeInOutBack;    
                    break;
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    
    // Create a circle
    sf::CircleShape circle = sf::CircleShape(10);  // Initialize a circle with radius 10
    
    // Set number of frames in the animation 
    int animationFrames = 60;

    // Set static variable frame - doesn't get reset between calls to render
    static int frameCount = 0;

    // Find y position
    float windowHeight = window.getSize().y;
    float yPosition = (windowHeight * 0.33);  // sets y position to about 1/3 from the top of the screen
    float xPosition = 0;

    // Circle moves from left to right of screen, so a and b in the tween function are going to
    // be the x positions at the left and right of the screen. Start x = 0
    float xEndPosition = window.getSize().x;    
    
    // Get the t between 0 to 1 from the frames - % makes it reset to 0 for the cycle, dividing normalizes it
    float t = (frameCount % animationFrames) / float(animationFrames);

    xPosition = tween(0, xEndPosition, t);

    ++frameCount;
    circle.setPosition(sf::Vector2f(xPosition, yPosition));
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
