#include <iostream>
#include <string>
//#include "vector.h"
#include "Vector2.h"
using namespace std;

//typedef Vector2<float> Vector2f;

int main()
{
    cout << "Hello!" << endl;
    
    Vector2f vec(5.4f, 7.1f);
    Vector2f vec2(3.3f, 1.2f);
    Vector2f vec3 = vec.add(vec2);
    vec3 = vec3.scale(2.0);
    
    cout << "Vector " << vec3.x << ", " << vec3.y << endl;
    return 0;
}
