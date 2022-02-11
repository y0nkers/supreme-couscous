# supreme-couscous
Console application for:
- calculating the distance traveled by a material point thrown at an angle to the horizon (with and without air resistance)
- determining the area of intersection of a circle centered at the origin and a rectangle centered at the origin
## Distance traveled by a material point
Model of motion of a material point thrown at an angle to the horizon:

![Model](https://user-images.githubusercontent.com/75331348/153642082-c8282041-9aad-4d72-b526-a827af98f7c1.png)

### Analytic model
Formula for distance traveled by a material point thrown at an angle to the horizon: 

![Formula1](https://sun9-15.userapi.com/impg/p6ZEYz0uNbNbRB06P6mgexWdjSqB7JPQDK2Caw/YqkPXep6pDM.jpg?size=135x53&quality=96&sign=a39ad5c2e861437fd8630cf65456d1c2&type=album "Analytic model formula")
### Imitation model
Formulas with air resistance (in our case m = 1)

![Formula2](https://sun9-62.userapi.com/impg/sEJLcI1uA_UaSZeNXnIQhw1sPzparCpjOcqqNg/3JZJCqEJ6IY.jpg?size=677x365&quality=96&sign=915d73eef0e64ad7cc2614645a503267&type=album "Imitation model formula")

If you want to plot a simulation model, compile plot.py to .exe (e.g. with pyinstaller) and put it into the directory along with main.cpp

Also define PLOT to 1 in main.cpp
```cpp
#define PLOT 1 // 1 if you want to plot
```

## Area of intersection of a circle and a rectangle
### 4 cases:
#### Case 1: Circle inside rectangle

![Case 1](https://sun9-61.userapi.com/impg/R11cKVRuI5ZAmAIaFRikU4If7XbtQ-6vZRHNZw/Hg8ZTblGXXc.jpg?size=317x383&quality=96&sign=28ddf718f69330c06d84aaa8ca64d36e&type=album "Case 1")
```cpp
analytic_area = PI * circle_radius * circle_radius;
```
#### Case 2: Rectangle inside circle

![Case 2](https://sun9-33.userapi.com/impg/49wjBd6v1LrpmhP8MVlTntP8Ge8EBoGdEwA17g/dVAuiTou9HA.jpg?size=382x426&quality=96&sign=e1608bc8ec5d2ca66e2016b0feda3a00&type=album "Case 2")
```cpp
analytic_area = rectangle_height * rectangle_width;
```
#### Case 3: Rectangle is wider than the circle, but **"lower"** than it

![Case 3](https://sun9-27.userapi.com/impg/0peCv7crmYeHDjqMYFn11S29xeg49BjoNqx4GA/WwbmjkTtsCg.jpg?size=499x364&quality=96&sign=76dbec62d058190eec6048b621164f2f&type=album "Case 3")
```cpp
long double cathetus_ab = sqrt((circle_radius * circle_radius) - half_height * half_height); // AB = sqrt(AO^2 - OB^2)
ABO_area = cathetus_ab * half_height / 2; // 1/2 * AB * OB
long double OAB_degree = acos(cathetus_ab / circle_radius) / PI * 180;
ACO_area = PI * circle_radius * circle_radius * OAB_degree / 360;
analytic_area = 4 * (ABO_area + ACO_area); // S = 4S_aboc = 4(S_abo + S_aoc)
```
#### Case 4: Rectangle is taller than the circle, but **"narrower"** than it

![Case 4](https://sun9-12.userapi.com/impg/k1mrZeOs9HdMUTX_IDk2pqVgdLQxp7ugH_fC7w/yC44Qy5RLFQ.jpg?size=325x442&quality=96&sign=997a3bbac96afa95ef1eaac4f4e17158&type=album "Case 4")
```cpp
long double cathetus_ac = sqrt((circle_radius * circle_radius) - half_width * half_width/ 2);
ACO_area = cathetus_ac * half_width / 2; // 1/2 * AC * OC
long double OAC_degree = acos(cathetus_ac / circle_radius) / PI * 180;
ABO_area = PI * circle_radius * circle_radius * OAC_degree / 360;
analytic_area = 4 * (ABO_area + ACO_area);
```
