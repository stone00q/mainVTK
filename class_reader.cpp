#include "class_reader.h"
void TecplotReader::pointsReader(int pointId, const std::string& line, int varNum, std::vector<vtkSmartPointer<vtkFloatArray>>& zoneData, vtkPoints* thePoints) {
    std::istringstream iss(line);
    std::string theValue;
    float x, y, z, tmp;
    for (int i = 0; i < varNum; i++)
    {
        iss >> theValue;
        tmp = stof(theValue);
        //std::cout << tmp << " ";
        zoneData[i]->SetValue(pointId, tmp);
        switch (i)
        {
        case 0:
            x = tmp;
            break;
        case 1:
            y = tmp;
            break;
        case 2:
            z = tmp;
            break;
        default:
            break;
        }
    }
    thePoints->InsertNextPoint(x, y, z);
}

void TecplotReader::cellsReader(const std::string& cellType, const std::string& line, vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid) {
    std::istringstream iss(line);
    std::string tmp;
    std::vector<int> realcell;
    auto ids = vtkSmartPointer<vtkIdList>::New();
    int cur = 0;
    while (iss >> tmp)
    {
        int theID = stoi(tmp) - 1;

        if (realcell.empty())
        {
            realcell.push_back(theID);
            ids->InsertNextId(theID);
        }
        else if (theID != realcell[cur]) {
            realcell.push_back(theID);
            cur++;
            ids->InsertNextId(theID);
        }
        //std::cout << "theID : " << theID << std::endl;
    }
    int len = realcell.size();
    switch (len)
    {
    case 3:
        unstructuredGrid->InsertNextCell(VTK_TRIANGLE, ids);
        break;
    case 4:
        if (cellType == "FEQUADRILATETAL")
            unstructuredGrid->InsertNextCell(VTK_QUAD, ids);
        else unstructuredGrid->InsertNextCell(VTK_TETRA, ids);
        break;
    case 5:
        unstructuredGrid->InsertNextCell(VTK_PYRAMID, ids);
        break;
    case 6:
        unstructuredGrid->InsertNextCell(VTK_WEDGE, ids);
        break;
    case 8:
        unstructuredGrid->InsertNextCell(VTK_HEXAHEDRON, ids);
        break;
    default:
        break;
    }
}

vtkMultiBlockDataSet* TecplotReader::readTecplot() {
    clock_t start_time = clock();

    std::ifstream file(this->fileName);
    std::string line;
    std::string currentToken;

    std::string title;
    int solutiontime = 0;
    int zoneNum = 0;
    int nodeNum; //the number of points

    std::vector<std::string> varName;

    std::vector<std::string> zoneTitle;
    std::vector<int> zoneCellNum;
    std::vector<std::string> zoneCellType;

    auto thePoints = vtkSmartPointer<vtkPoints>::New();
    std::vector<vtkSmartPointer<vtkFloatArray>> zoneData;
    //auto pointsData = vtkSmartPointer<vtkPoints>::New();
    vtkMultiBlockDataSet* multiBlock = vtkMultiBlockDataSet::New();
    auto sharedPointData = vtkSmartPointer<vtkPointData>::New();

    bool pointsOK = false;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        if (!pointsOK)
        {
            if (line.find("TITLE") != std::string::npos)
            {
                int pos1 = line.find("\"");
                int pos2 = line.find("\"", pos1 + 1);
                title = line.substr(pos1 + 1, pos2 - pos1 - 1);
                //std::cout << title << std::endl;
                continue;
            }

            if (line.find("VARIABLES") != std::string::npos)
            {
                size_t pos1 = line.find("\"");
                size_t pos2 = line.find("\"", pos1 + 1);
                std::string tempVarName;
                while (pos2 != std::string::npos) {
                    tempVarName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    //std::cout << tempVarName << std::endl;
                    varName.push_back(tempVarName);
                    if (pos2 == line.size() - 1) break;
                    pos1 = line.find("\"", pos2 + 1);
                    pos2 = line.find("\"", pos1 + 1);
                }
                continue;
            }

            if (line.find("solutiontime") != std::string::npos)
            {
                std::istringstream iss(line);
                while (iss >> currentToken) {
                    if (isdigit(currentToken[0]))
                    {
                        solutiontime = std::stoi(currentToken);
                        break;
                    }
                }
                continue; //
            }

            if (line.find("ZONE") != std::string::npos)
            {
                zoneNum++;
                std::replace(line.begin(), line.end(), '=', ' ');
                std::replace(line.begin(), line.end(), ',', ' ');
                std::replace(line.begin(), line.end(), '"', ' ');
                std::istringstream iss(line);

                while (iss >> currentToken)
                {
                    if (currentToken == "T")
                    {
                        iss >> currentToken;
                        //std::cout <<"T=" <<currentToken << std::endl;
                        zoneTitle.push_back(currentToken);
                    }
                    else if (currentToken == "N")
                    {
                        iss >> nodeNum;
                        //std::cout <<"N="<< nodeNum << std::endl;
                    }
                    else if (currentToken == "E")
                    {
                        iss >> currentToken;
                        //std::cout <<"E="<< currentToken << std::endl;
                        zoneCellNum.push_back(std::stoi(currentToken));
                    }
                    else if (currentToken == "ZONETYPE")
                    {
                        iss >> currentToken;
                        zoneCellType.push_back(currentToken);
                        break;
                    }
                }
                continue;
            }

            int varNum = varName.size();

            for (int i = 0; i < varNum; i++) {
                vtkFloatArray* theArray = vtkFloatArray::New();
                theArray->SetNumberOfTuples(nodeNum);
                theArray->SetName(varName[i].c_str());
                zoneData.push_back(theArray);
                theArray->Delete();
            }
            pointsReader(0, line, varNum, zoneData, thePoints);


            for (int i = 1; i < nodeNum; i++)
            {
                std::getline(file, line);
                pointsReader(i, line, varNum, zoneData, thePoints);
            }
            pointsOK = true;
            continue;
        }

        if (line.find("ZONE") != std::string::npos)
        {
            zoneNum++;

            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::replace(line.begin(), line.end(), ',', ' ');
            std::istringstream iss(line);
            //std::cout << line << std::endl;
            while (iss >> currentToken)
            {
                if (currentToken == "T")
                {
                    iss >> currentToken;
                    //std::cout << currentToken << std::endl;
                    zoneTitle.push_back(currentToken);
                }
                else if (currentToken == "E")
                {
                    iss >> currentToken;
                    //std::cout << currentToken << std::endl;
                    zoneCellNum.push_back(std::stoi(currentToken));
                }
                else if (currentToken == "ZONETYPE")
                {
                    iss >> currentToken;
                    zoneCellType.push_back(currentToken);
                    break;
                }
            }
            continue;
        }

        auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ug->SetPoints(thePoints);


        if (zoneNum == 1) {
            for (auto& data : zoneData) {
                ug->GetPointData()->AddArray(data);
            }
            auto calculator = vtkSmartPointer<vtkArrayCalculator>::New();
            calculator->SetInputData(ug);
            calculator->AddScalarArrayName("u");
            calculator->AddScalarArrayName("v");
            calculator->AddScalarArrayName("w");
            calculator->SetResultArrayName("velocity");
            calculator->SetFunction("u*iHat + v*jHat + w*kHat");
            calculator->Update();

            ug = calculator->GetUnstructuredGridOutput();
            sharedPointData = ug->GetPointData();
        }
        else {
            ug->GetPointData()->ShallowCopy(sharedPointData);
        }

        int zoneId = zoneNum - 1;
        //std::cout << "zoneId = " << zoneId << std::endl;
        cellsReader(zoneCellType[zoneId], line, ug);
        for (int i = 1; i < zoneCellNum[zoneId]; i++)
        {
            std::getline(file, line);
            cellsReader(zoneCellType[zoneId], line, ug);
        }
        multiBlock->SetBlock(zoneId, ug);
    }

    clock_t end_time = clock();
    std::cout << "" << (end_time - start_time) / (double)CLOCKS_PER_SEC << "s" << std::endl;

    return multiBlock;
}
