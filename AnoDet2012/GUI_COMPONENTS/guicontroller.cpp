#include "guicontroller.h"

GUIController::GUIController()
{
}

GUIController::GUIController(LiveLineChart *liveLineChart) :
    liveLineChart(liveLineChart)
{
}

void GUIController::setLiveLineChart(LiveLineChart *liveLineChart)
{
    this->liveLineChart = liveLineChart;
}

void GUIController::refreshLiveLineChart()
{
    this->liveLineChart->reloadData();
}
