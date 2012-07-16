#ifndef FB2HEAD_H
#define FB2HEAD_H

#include <QAbstractItemModel>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>
#include <QTreeView>
#include <QWebElement>
#include <QWebView>

QT_BEGIN_NAMESPACE
class QAction;
class QToolBar;
QT_END_NAMESPACE

#include "fb2xml.h"

class Fb2TextEdit;

class Fb2Scheme : public QDomElement
{
    FB2_BEGIN_KEYLIST
        XsElement,
        XsChoice,
        XsComplexType,
        XsSequence,
    FB2_END_KEYLIST

private:
    class Fb2 : public QDomDocument { public: Fb2(); };

public:
    Fb2Scheme() {}
    Fb2Scheme(const Fb2Scheme &x) : QDomElement(x) {}
    Fb2Scheme(const QDomElement &x) : QDomElement(x) {}
    Fb2Scheme& operator=(const Fb2Scheme &x) { QDomElement::operator=(x); return *this; }

    static const QDomDocument & fb2();
    Fb2Scheme element(const QString &name) const;
    void items(QStringList &list) const;
    QString info() const;
    QString type() const;

private:
    Fb2Scheme typeScheme() const;
};

class Fb2HeadItem: public QObject
{
    Q_OBJECT

    FB2_BEGIN_KEYLIST
        Auth,
        Cover,
        Image,
        Seqn,
    FB2_END_KEYLIST

public:
    explicit Fb2HeadItem(QWebElement &element, Fb2HeadItem *parent = 0);

    virtual ~Fb2HeadItem();

    Fb2HeadItem * append(const QString name);

    void remove(int row);

    Fb2HeadItem * item(const QModelIndex &index) const;

    Fb2HeadItem * item(int row) const;

    int index(Fb2HeadItem * child) const {
        return m_list.indexOf(child);
    }

    int count() const {
        return m_list.size();
    }

    Fb2HeadItem * parent() const {
        return m_parent;
    }

    QString text(int col = 0) const;

    void setText(const QString &text);

    const QString & id() const {
        return m_id;
    }

    const QString & name() const {
        return m_name;
    }

    QString sub(const QString &key) const;

    Fb2Scheme scheme() const;

private:
    class HintHash : public QHash<QString, QString>
    {
    public:
        explicit HintHash();
    };

private:
    void addChildren(QWebElement &parent);
    QString value() const;
    QString hint() const;

private:
    QList<Fb2HeadItem*> m_list;
    QWebElement m_element;
    Fb2HeadItem * m_parent;
    QString m_name;
    QString m_text;
    QString m_id;
};

class Fb2HeadModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit Fb2HeadModel(QWebView &view, QObject *parent = 0);
    virtual ~Fb2HeadModel();
    void expand(QTreeView *view);
    Fb2HeadItem * item(const QModelIndex &index) const;
    QModelIndex append(const QModelIndex &parent, const QString &name);
    void remove(const QModelIndex &index);

public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QWebView & m_view;
    Fb2HeadItem * m_root;
};

class Fb2HeadView : public QTreeView
{
    Q_OBJECT

public:
    explicit Fb2HeadView(Fb2TextEdit &view, QWidget *parent = 0);
    void initToolbar(QToolBar &toolbar);

signals:
    void status(const QString &text);

public slots:
    void editCurrent();
    void updateTree();

private slots:
    void activated(const QModelIndex &index);
    void collapsed(const QModelIndex &index);
    void appendNode();
    void removeNode();

protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void showStatus(const QModelIndex &current);

private:
    Fb2TextEdit & m_view;
    QAction * actionInsert;
    QAction * actionModify;
    QAction * actionDelete;
};

class Fb2NodeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Fb2NodeDlg(QWidget *parent, Fb2Scheme scheme, QStringList &list);
    QString value() const;

private slots:
    void comboChanged(const QString &text);

private:
    const Fb2Scheme m_scheme;
    QComboBox * m_combo;
    QLabel * m_text;
};

#endif // FB2HEAD_H
