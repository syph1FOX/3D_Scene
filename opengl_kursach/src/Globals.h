#pragma once
#include <glm/glm.hpp>

//Набор глобальных переменных и констант для управления сценой

#define PI 3.14159265358979323846f 
/// <summary>
/// Ширина окна.
/// </summary>
extern int WinWidth;
/// <summary>
/// Высота окна.
/// </summary>
extern int WinHeight;

/// <summary>
/// Единичная матрица. Для быстрого сброса значений.
/// </summary>
extern const glm::mat4 WorldMatrix;
/// <summary>
/// Направление вращения модели.
/// </summary>
extern bool rotationmode;
