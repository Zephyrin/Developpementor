#ifndef ENVIRONMENTSTRING2_H
#define ENVIRONMENTSTRING2_H

#include <environmentstring.h>
#include <QObject>

class EnvironmentString2 : public EnvironmentString
{
    Q_OBJECT
public:
  EnvironmentString2(const std::vector<int> board
                     =
 //  {2, 1, 5, 4, 1, 3, 5, 3, 1, 5, 6, 3, 5, 4, 7, 3, 5, 3, 9, 5});
   {9, 9, 8, 8, 7, 7, 5, 6, 6, 5, 4, 3, 3, 2, 2, 1, 1, 5, 9});
    EnvironmentString2(const QString &filename);
  ~EnvironmentString2();
    void init();

    const Interaction *getResult(const Interaction *intended);
protected:
    const Interaction *swapEq;
    const Interaction *feelEq;
    const Interaction *stepEq;
};

#endif // ENVIRONMENTSTRING2_H
