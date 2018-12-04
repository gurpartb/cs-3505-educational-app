#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "dog.h"

class Model: public QObject
{
    Q_OBJECT

public:
    Model();
    virtual ~Model();

private:
    void updateLevels();

public slots:
    void dogPetted();
    void dogFed();
    void dogPlayedWithBall();
    void dogWentToThePark();
    void dogLetOut();

signals:
    void updateTrustLevel(int);
    void updateHungerLevel(int);
    void updateBathroomLevel(int);

private:
    Dog myDog;
};

#endif // MODEL_H
