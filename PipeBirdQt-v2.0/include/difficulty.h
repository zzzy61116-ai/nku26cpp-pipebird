#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <QString>
#include <QVector>

struct DifficultyConfig
{
    QString name;
    double gravity;
    double flapStrength;
    double pipeSpeed;
    int gapHeight;
    int spawnIntervalFrames;
};

inline QVector<DifficultyConfig> difficultyList()
{
    return {
        { "简单",   820.0, -335.0, 175.0, 185, 105 },
        { "普通",   920.0, -350.0, 220.0, 165, 90  },
        { "困难",  1030.0, -365.0, 270.0, 145, 78  }
    };
}

#endif
