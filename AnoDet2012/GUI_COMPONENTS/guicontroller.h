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


    // another component...
    // ...

private:
    LiveLineChart *liveLineChart;
};

#endif // GUICONTROLLER_H
