TEMPLATE = app
TARGET = Recomment
DEPENDPATH += .

blackberry {
BBSIMONECOMMONPATH = ../build-BBSimoneShared-Qt_4_8_3_prefix-Release
}
unix:!blackberry {
BBSIMONECOMMONPATH = ../build-BBSimoneShared-Desktop-Debug
}

QT += declarative xml mobility

OTHER_FILES += \
        src/qml/recomment/main.qml \
        src/qml/recomment/EditableComboBox.qml \
        src/qml/recomment/ComboBox.qml \
        src/qml/recomment/Button.qml \
        src/qml/recomment/LineEdit.qml \
        src/qml/recomment/CheckBox.qml \
        src/qml/recomment/AnimatedItem.qml \
        src/qml/recomment/ProgressBar.qml \
        src/qml/recomment/Dialog.qml \
        src/qml/recomment/ConnectionSettings.qml \
        src/qml/recomment/ActiveConsole.qml \
    src/qml/recomment/Offer.qml \
    src/qml/recomment/AttributeDelegate.qml \
    src/qml/recomment/AnimatedText.qml


blackberry {
	QMAKE_LFLAGS += '-Wl,-rpath,\'./app/native/lib\''
	PACKAGE_ARGS = \
                $${PWD}/bar-descriptor.xml $$TARGET \
                -e $${PWD}/recomment86.png res/recomment86.png \
                -e $${BBSIMONECOMMONPATH}/libSimoneShared.so.1.0.0 lib/libSimoneShared.so.1 \
		-e $$[QT_INSTALL_LIBS]/libQtCore.so.4 lib/libQtCore.so.4 \
		-e $$[QT_INSTALL_LIBS]/libQtGui.so.4 lib/libQtGui.so.4 \
		-e $$[QT_INSTALL_LIBS]/libQtOpenGL.so.4 lib/libQtOpenGL.so.4 \
		-e $$[QT_INSTALL_LIBS]/libQtNetwork.so.4 lib/libQtNetwork.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtDeclarative.so.4 lib/libQtDeclarative.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtSql.so.4 lib/libQtSql.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtSvg.so.4 lib/libQtSvg.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtScript.so.4 lib/libQtScript.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtXmlPatterns.so.4 lib/libQtXmlPatterns.so.4 \
                -e $$[QT_INSTALL_LIBS]/libQtXml.so.4 lib/libQtXml.so.4 \
                -e $$[QT_INSTALL_PLUGINS]/platforms/libblackberry.so plugins/platforms/libblackberry.so \
                -e $${PWD}/src/qml/ res/qml/ \
                -e $${PWD}/db/ res/db/

	package.target = $${TARGET}.bar
	package.depends = $$TARGET
	package.commands = blackberry-nativepackager \
		-package $${TARGET}.bar \
		-devMode -debugToken $$(DEBUG_TOKEN) \
		$${PACKAGE_ARGS}

	QMAKE_EXTRA_TARGETS += package

	OTHER_FILES += bar-descriptor.xml
	OTHER_FILES += recomment86.png
}

SOURCES += src/main.cpp \
        src/recommentview.cpp \
        src/qmlrecommentview.cpp \
    src/numericalattribute.cpp \
    src/stringattribute.cpp \
    src/relationship.cpp \
    src/critique.cpp \
    src/offer.cpp \
    src/valueattribute.cpp \
    src/critiquerecommender.cpp \
    src/recomment.cpp \
    src/attributemodel.cpp \
    src/attributefactory.cpp \
    src/compoundattribute.cpp

HEADERS += \
        src/recommentview.h \
        src/qmlrecommentview.h \ 
    src/attribute.h \
    src/numericalattribute.h \
    src/stringattribute.h \
    src/relationship.h \
    src/critique.h \
    src/offer.h \
    src/valueattribute.h \
    src/critiquerecommender.h \
    src/recomment.h \
    src/attributemodel.h \
    src/attributefactory.h \
    src/compoundattribute.h \
    src/attributecreators.h


INCLUDEPATH += ../BBSimoneShared
LIBS += -L$${BBSIMONECOMMONPATH} -lSimoneShared
