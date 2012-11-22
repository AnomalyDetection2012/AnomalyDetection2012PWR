#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "livelinechart.h"

class GUIController
{
public:
    GUIController();    // remember to use setters to set components
    GUIController(LiveLineChart *liveLineChart);    // TODO: add another components



    // BEGIN LiveLineChart component:
        void setLiveLineChart(LiveLineChart *liveLineChart);

        void refreshLiveLineChart();
        void setLogScaleLiveLineChart();
        void setLinearScaleLiveLineChart();

    // END LiveLineChart


    // BEGIN Test LiveLineChart

    // END Test LiveLineChart


    // another component...
    // ...

public:
    LiveLineChart *liveLineChart;
    LiveLineChart *testLineChart;
};

#endif // GUICONTROLLER_H
