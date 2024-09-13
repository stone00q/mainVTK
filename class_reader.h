#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCellType.h>
#include <ctime>

#include <vtkArrayCalculator.h>

class TecplotReader {
private:
    std::string fileName;

    // Helper functions:
    void pointsReader(int pointId, const std::string& line, int varNum, std::vector<vtkSmartPointer<vtkFloatArray>>& zoneData, vtkPoints* thePoints);
    void cellsReader(const std::string& cellType, const std::string& line, vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid);

public:
    TecplotReader(const std::string& fileName) : fileName(fileName) {}
    vtkMultiBlockDataSet* readTecplot();
};

