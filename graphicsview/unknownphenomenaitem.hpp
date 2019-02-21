#ifndef PHENOMENAITEM_H
#define PHENOMENAITEM_H

#include "phenomenaitem.hpp"

class UnknownPhenomenaItem : public PhenomenaItem
{
public:
  UnknownPhenomenaItem(const BuildGraphics &build
              , GraphicsView *graphWidget
              , const std::vector<const UsageTable *> &lNoema);
  ~UnknownPhenomenaItem();
  enum { Type = UserType + 2 };
  int type() const Q_DECL_OVERRIDE { return Type; }

  void updateNode();
  const PhenomenaItem *getBelieve() const;
  bool hasNoema(const std::string &id) const;
  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter
             , const QStyleOptionGraphicsItem *option
             , QWidget *);
protected:
  void interactionHasBelieve(const UsageTable *noema);
  void interactionHasNoBelieve(const UsageTable *noema);
private :
  QList<const UsageTable *> noemaUnknown;
  QMap<const UsageTable *, QList<PhenomenaEdge *> > believeListDeprecated;
  QMap<const UsageTable *, PhenomenaItem *> believeList;
};

#endif // PHENOMENAITEM_H
