QT       += core gui
QT       += charts
QT       += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Num_Lib/Basic_Class/basic_class.cpp \
    Num_Lib/CPM/find_neighborhood.cpp \
    Num_Lib/CPM/hamiltonian.cpp \
    Num_Lib/CPM/plot_cpm.cpp \
    Num_Lib/CPM/initialization_cell.cpp \
    Num_Lib/CRS/crs_output.cpp \
    Num_Lib/CRS/ssa_solver.cpp \
    Num_Lib/CRS/ode_solver.cpp \
    Num_Lib/CRS/crs_initialization.cpp \
    Num_Lib/Motor_Clutch/mainloop.cpp \
    Num_Lib/Motor_Clutch/mcm_initialization.cpp \
    Num_Lib/Motor_Clutch/mcm_output.cpp \
    UI/cpm.cpp \
    UI/hybrid.cpp \
    UI/hybrid_ui.cpp \
    UI/mcm.cpp \
    UI/mcm_ui.cpp \
    UI/mainwindow.cpp \
    main.cpp \
    QCustomPlot/qcustomplot.cpp \

HEADERS += \
    Num_Lib/Basic_Class/basic_class.h \
    Num_Lib/CPM/find_neighborhood.h \
    Num_Lib/CPM/hamiltonian.h \
    Num_Lib/CPM/plot_cpm.h \
    Num_Lib/CPM/initialization_cell.h \
    Num_Lib/CRS/crs_output.h \
    Num_Lib/CRS/ssa_solver.h \
    Num_Lib/CRS/ode_solver.h \
    Num_Lib/CRS/crs_initialization.h \
    Num_Lib/CRS/parameters.h \
    Num_Lib/CRS/reaction.h \
    Num_Lib/CRS/iteration.h \
    Num_Lib/Motor_Clutch/mainloop.h \
    Num_Lib/Motor_Clutch/mcm_initialization.h \
    Num_Lib/Motor_Clutch/mcm_output.h \
    Num_Lib/Motor_Clutch/parameters.h \
    Num_Lib/Motor_Clutch/iteration.h \
    UI/hybrid.h \
    UI/hybrid_ui.h \
    UI/mcm.h \
    UI/mcm_ui.h \
    UI/cpm.h \
    UI/mainwindow.h \
    QCustomPlot/qcustomplot.h \

FORMS += \
    UI/hybrid.ui \
    UI/mcm.ui \
    UI/cpm.ui \
    UI/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    UI/Resource_Figs.qrc
