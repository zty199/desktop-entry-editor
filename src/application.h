#ifndef APPLICATION_H
#define APPLICATION_H

#include <DApplication>

class Application : public Dtk::Widget::DApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);

    /**
     * @brief Using DLogManager to register log appender
     */
    static void registerLogAppender();

protected:
    void handleAboutAction() override;

private:
    void initAboutDialog();
#if DTK_VERSION >= DTK_VERSION_CHECK(5, 6, 4, 0)
    void initFeatureDisplayDialog();
#endif
};

#endif // APPLICATION_H
