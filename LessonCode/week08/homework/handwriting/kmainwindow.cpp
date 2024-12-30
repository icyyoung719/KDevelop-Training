#include "kmainwindow.h"

KMainWindow::KMainWindow(QWidget* parent)
	: QMainWindow(parent), tsfManager(new TSFManager())
{
    ui.setupUi(this);
	connectSignals();
    init();
}

KMainWindow::~KMainWindow() {
    delete tsfManager;
}

void KMainWindow::init() {
    updateButtonLabels(ScribbleArea::defaultRecognitionResults); // ������ʾ��Ĭ������
    updateUndoButton(false); // ���³�����ť״̬
    updateClearButton(false); // ������հ�ť״̬
    updateCustomColorButtonColor(); // ����showCustomColorButton����ɫ
    ui.tabWidget->tabBar()->hide(); // ����tabBar
    ui.autoModeButton->clicked(); // Ĭ���Զ�ʶ��ģʽ
    ui.nonChineseInputWidget->hide(); // ���ط����������
    ui.lineWidthSpinBox->setValue(4); // ����Ĭ�ϵ��߿�
    ui.blackColorButton->clicked();  // ����Ĭ�ϵıʼ���ɫΪ��ɫ
    updateButtonFontSize(0); // ���ð�ť�������СΪС
    setWindowFlags(Qt::WindowStaysOnTopHint); // ���ô���ʼ�������ϲ�
}

void KMainWindow::connectSignals() {
    connect(ui.scribblearea, &ScribbleArea::recognitionResults, this, &KMainWindow::updateButtonLabels);
    connect(ui.scribblearea, &ScribbleArea::canUndoChanged, this, &KMainWindow::updateUndoButton);
    connect(ui.scribblearea, &ScribbleArea::canClearChanged, this, &KMainWindow::updateClearButton);

    // ���Ӱ�ť
    connect(ui.undoOneStrokeButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::undo);
    connect(ui.clearStrokesButton, &QPushButton::pressed, ui.scribblearea, &ScribbleArea::clear);

    // ʶ��ģʽ��ť
    connect(ui.autoModeButton, &QPushButton::clicked, this, &KMainWindow::updateRecognitionMode);
    connect(ui.singleWordPushButton, &QPushButton::clicked, this, &KMainWindow::updateRecognitionMode);
    connect(ui.sentencePushButton, &QPushButton::clicked, this, &KMainWindow::updateRecognitionMode);

    // �������ְ�ť����źŵ��ۺ���
    connect(ui.resultTextButton_1, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_2, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_3, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_4, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_5, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_6, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_7, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_8, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_9, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.resultTextButton_10, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.longTextButton, &SendTextButton::sendText, this, &KMainWindow::sendText);
    connect(ui.longTextButton_2, &SendTextButton::sendText, this, &KMainWindow::sendText);


    // ���������밴ť
    connect(ui.letterModeButton, &QPushButton::clicked, this, &KMainWindow::showNonChineseInputWidget);
    connect(ui.numberModeButton, &QPushButton::clicked, this, &KMainWindow::showNonChineseInputWidget);
    connect(ui.punctuationModeButton, &QPushButton::clicked, this, &KMainWindow::showNonChineseInputWidget);
    connect(ui.ChineseModeButton, &QPushButton::clicked, this, &KMainWindow::hideNonChineseInputWidget);

    // �� R G B ������������ valueChanged �ź����ӵ� updateCustomColorButtonColor �ۺ��� 
    connect(ui.RValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.GValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));
    connect(ui.BValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomColorButtonColor()));

    // ��lineWidthSpinBox��valueChanged�ź����ӵ�updateCustomWidthLineWidth�ۺ���
    connect(ui.lineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCustomWidthLineWidth()));
    // ��wordWidthComboBox��valueChanged�ź����ӵ�updateButtonFontSize�ۺ���
    connect(ui.wordWidthComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KMainWindow::updateButtonFontSize);

    // ��colorButton��clicked�ź����ӵ�ScribbleArea��setPenColor�ۺ���
    connect(ui.blackColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.redColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.yellowColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.greenColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.showCustomColorButton, &QPushButton::clicked, this, &KMainWindow::setPenColorFromButton);
    connect(ui.chooseCustomColorButton, &QPushButton::clicked, this, [this]() { // chooseCustomColorButton����showCustomColorButton
        ui.showCustomColorButton->click();
        });

    // ��settingPushButton��returnHandwritingPushButton��clicked�ź����ӵ�kmainwindow��ҳ���л�
    connect(ui.settingPushButton, &QPushButton::clicked, this, [this]() {
        ui.tabWidget->setCurrentIndex(1);
        });
    connect(ui.returnHandwritingPushButton, &QPushButton::clicked, this, [this]() {
        ui.tabWidget->setCurrentIndex(0);
        });
    
    // �ָ���ʼ����
    connect(ui.setDefaultPreferenceButton, &QPushButton::clicked, this, &KMainWindow::setDefaultPerferences);
}

void KMainWindow::updateButtonLabels(const QStringList& labels) {
    if (labels.size() >= 10) {
        ui.resultTextButton_1->setText(labels.at(0));
        ui.resultTextButton_2->setText(labels.at(1));
        ui.resultTextButton_3->setText(labels.at(2));
        ui.resultTextButton_4->setText(labels.at(3));
        ui.resultTextButton_5->setText(labels.at(4));
        ui.resultTextButton_6->setText(labels.at(5));
        ui.resultTextButton_7->setText(labels.at(6));
        ui.resultTextButton_8->setText(labels.at(7));
        ui.resultTextButton_9->setText(labels.at(8));
        ui.resultTextButton_10->setText(labels.at(9));
    }
}

void KMainWindow::updateUndoButton(bool canUndo) {
    ui.undoOneStrokeButton->setEnabled(canUndo); // ���³�����ť�Ƿ����
}

void KMainWindow::mousePressEvent(QMouseEvent* event) {
    // ��ֹĬ�ϵľ۽���Ϊ

    // ����Windows API���ֹ���ڴ������Ծ
    //POINT cursorPos;
    //GetCursorPos(&cursorPos);
    
    QMainWindow::mousePressEvent(event);
    //SetCursorPos(cursorPos.x, cursorPos.y);

}

void KMainWindow::updateClearButton(bool canClear) {
    ui.clearStrokesButton->setEnabled(canClear); // ������հ�ť�Ƿ����
}

void KMainWindow::updateCustomColorButtonColor()
{
    // ��ȡ��������ֵ
    int r = ui.RValueSpinBox->value();
    int g = ui.GValueSpinBox->value();
    int b = ui.BValueSpinBox->value();

    // ��ȡ��ǰ��ʽ��
    QString currentStyleSheet = ui.showCustomColorButton->styleSheet();

    // �����µı�����ɫ��ʽ
    QString newBackgroundColor = QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b);

    // ʹ��������ʽ���±�����ɫ��ʽ
    QRegExp backgroundColorPattern("background-color: rgb\\(\\d+, \\d+, \\d+\\);");
    if (backgroundColorPattern.indexIn(currentStyleSheet) != -1) {
        // ������ڱ�����ɫ��ʽ�����滻
        currentStyleSheet.replace(backgroundColorPattern, newBackgroundColor);
    }
    else {
        // ��������ڱ�����ɫ��ʽ����׷��
        currentStyleSheet.append(newBackgroundColor);
    }

    // �����µ���ʽ��
    ui.showCustomColorButton->setStyleSheet(currentStyleSheet);
}

void KMainWindow::setPenColorFromButton() {
    static QList<QPushButton*> buttons = {
        ui.blackColorButton,
        ui.redColorButton,
        ui.yellowColorButton,
        ui.greenColorButton,
        ui.showCustomColorButton
    };
    // ��ȡ�����źŵİ�ť
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        button->setDisabled(true);
        for (QPushButton* b : buttons) {
            if (b != button) {
                b->setEnabled(true);
            }
        }
        // ��ȡ��ť�ı�����ɫ
        QColor color = button->palette().color(QPalette::Button);
        // ���ñʼ���ɫ
        ui.scribblearea->setPenColor(color);
    }
}

void KMainWindow::updateCustomWidthLineWidth() {
	// ��ȡ��������ֵ
	int width = ui.lineWidthSpinBox->value()*20;
	// ������ʾ���ߵĴ�ϸ
	ui.showCustomWidthLine->setMidLineWidth(width);
	// ���±ʵĴ�ϸ
	ui.scribblearea->setPenWidth(width);
    // TODO!!!!!
}

void KMainWindow::updateRecognitionMode() {
    // ������ť��ʶ��ģʽ��ӳ��
    static const QHash<QPushButton*, ScribbleArea::RecognitionMode> modeMap = {
        {ui.autoModeButton, ScribbleArea::RecognitionMode::Auto},
        {ui.singleWordPushButton, ScribbleArea::RecognitionMode::Word},
        {ui.sentencePushButton, ScribbleArea::RecognitionMode::Sentence}
    };

    // ��ȡ�����źŵİ�ť
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button && modeMap.contains(button)) {
        ui.scribblearea->setRecognitionMode(modeMap.value(button));
		button->setDisabled(true);  // ���ð�ť������
        // ����������ť����
        for (auto it = modeMap.begin(); it != modeMap.end(); ++it) {
            if (it.key() != button) {
                it.key()->setEnabled(true);
            }
        }
    }
    else { // error
		qDebug() << "Error: updateRecognitionMode()";
    }
}

void KMainWindow::showNonChineseInputWidget() {
	// ������ť�ͷ���������ģʽ��ӳ��
	static const QHash<QPushButton*, NonChineseInputMode> modeMap = {
		{ui.numberModeButton, NonChineseInputMode::Number},
		{ui.letterModeButton, NonChineseInputMode::Letter},
		{ui.punctuationModeButton, NonChineseInputMode::Punctuation}
	};

	// ��ȡ�����źŵİ�ť
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button && modeMap.contains(button)) {
		// ���·��������������
        updateNonChineseWidget(modeMap.value(button));
		// ��ʾ�����������
		ui.nonChineseInputWidget->show();
		// ���ð�ť������
		button->setDisabled(true);
		// ����������ť����
		for (auto it = modeMap.begin(); it != modeMap.end(); ++it) {
			if (it.key() != button) {
				it.key()->setEnabled(true);
			}
		}
        ui.ChineseModeButton->setEnabled(true);
	}
	else { // error
		qDebug() << "Error: showNonChineseInputWidget()";
	}
}

void KMainWindow::hideNonChineseInputWidget() {
	// ���ط����������
	ui.nonChineseInputWidget->hide();
	// ���ð�ť����
	ui.numberModeButton->setEnabled(true);
	ui.letterModeButton->setEnabled(true);
	ui.punctuationModeButton->setEnabled(true);
    ui.ChineseModeButton->setEnabled(false);
}

void KMainWindow::updateNonChineseWidget(NonChineseInputMode mode) {
	// ������ť�ͷ���������ģʽ��ӳ��
    static const QHash<NonChineseInputMode, QStringList> modeMap = [] {
        auto convert = [](const std::vector<const wchar_t*>& wcharList) {
            QStringList qStringList;
            std::transform(wcharList.begin(), wcharList.end(), std::back_inserter(qStringList),
                [](const wchar_t* wstr) { return QString::fromWCharArray(wstr); });
            return qStringList;
            };
        return QHash<NonChineseInputMode, QStringList>{
            {NonChineseInputMode::Number, convert({
                L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",
                L"9", L"", L"", L"", L"", L"", L"", L"", L"", 
                L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��",
                L"ʮ", L"��", L"", L"", L"", L"", L"", L"", L"", 
                L"Ҽ", L"��", L"��", L"��", L"��", L"½", L"��", L"��", L"��",
                L"ʰ", L"��", L"Ǫ", L"��", L"", L"", L"", L"", L"" })},
            { NonChineseInputMode::Letter, convert({
                L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i",
                L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"q", L"r",
                L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L" ",
                L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I",
                L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R",
                L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L" "}) },
            { NonChineseInputMode::Punctuation, convert({
                L",", L".", L"?", L"!", L":", L";", L"\"", L"'", L"(", L")",
                L"-", L"_", L"=", L"+", L"~", L"`", L"^", L"*", L"/", L"\\",
                L"[", L"]", L"{", L"}", L"<", L">", L"|", L"��", L"��", L"��",
                L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��",
                L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��",
                L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��"}) }
        };
        }();

	// ���°�ť����ʾ����
    QStringList texts = modeMap.value(mode);

    ui.nonChineseSendButton1_1->setText(texts.at(0));
    ui.nonChineseSendButton1_2->setText(texts.at(1));
    ui.nonChineseSendButton1_3->setText(texts.at(2));
    ui.nonChineseSendButton1_4->setText(texts.at(3));
    ui.nonChineseSendButton1_5->setText(texts.at(4));
    ui.nonChineseSendButton1_6->setText(texts.at(5));
    ui.nonChineseSendButton1_7->setText(texts.at(6));
    ui.nonChineseSendButton1_8->setText(texts.at(7));
    ui.nonChineseSendButton1_9->setText(texts.at(8));
    ui.nonChineseSendButton2_1->setText(texts.at(9));
    ui.nonChineseSendButton2_2->setText(texts.at(10));
    ui.nonChineseSendButton2_3->setText(texts.at(11));
    ui.nonChineseSendButton2_4->setText(texts.at(12));
    ui.nonChineseSendButton2_5->setText(texts.at(13));
    ui.nonChineseSendButton2_6->setText(texts.at(14));
    ui.nonChineseSendButton2_7->setText(texts.at(15));
    ui.nonChineseSendButton2_8->setText(texts.at(16));
    ui.nonChineseSendButton2_9->setText(texts.at(17));
    ui.nonChineseSendButton3_1->setText(texts.at(18));
    ui.nonChineseSendButton3_2->setText(texts.at(19));
    ui.nonChineseSendButton3_3->setText(texts.at(20));
    ui.nonChineseSendButton3_4->setText(texts.at(21));
    ui.nonChineseSendButton3_5->setText(texts.at(22));
    ui.nonChineseSendButton3_6->setText(texts.at(23));
    ui.nonChineseSendButton3_7->setText(texts.at(24));
    ui.nonChineseSendButton3_8->setText(texts.at(25));
    ui.nonChineseSendButton3_9->setText(texts.at(26));
    ui.nonChineseSendButton4_1->setText(texts.at(27));
    ui.nonChineseSendButton4_2->setText(texts.at(28));
    ui.nonChineseSendButton4_3->setText(texts.at(29));
    ui.nonChineseSendButton4_4->setText(texts.at(30));
    ui.nonChineseSendButton4_5->setText(texts.at(31));
    ui.nonChineseSendButton4_6->setText(texts.at(32));
    ui.nonChineseSendButton4_7->setText(texts.at(33));
    ui.nonChineseSendButton4_8->setText(texts.at(34));
    ui.nonChineseSendButton4_9->setText(texts.at(35));
    ui.nonChineseSendButton5_1->setText(texts.at(36));
    ui.nonChineseSendButton5_2->setText(texts.at(37));
    ui.nonChineseSendButton5_3->setText(texts.at(38));
    ui.nonChineseSendButton5_4->setText(texts.at(39));
    ui.nonChineseSendButton5_5->setText(texts.at(40));
    ui.nonChineseSendButton5_6->setText(texts.at(41));
    ui.nonChineseSendButton5_7->setText(texts.at(42));
    ui.nonChineseSendButton5_8->setText(texts.at(43));
    ui.nonChineseSendButton5_9->setText(texts.at(44));
    ui.nonChineseSendButton6_1->setText(texts.at(45));
    ui.nonChineseSendButton6_2->setText(texts.at(46));
    ui.nonChineseSendButton6_3->setText(texts.at(47));
    ui.nonChineseSendButton6_4->setText(texts.at(48));
    ui.nonChineseSendButton6_5->setText(texts.at(49));
    ui.nonChineseSendButton6_6->setText(texts.at(50));
    ui.nonChineseSendButton6_7->setText(texts.at(51));
    ui.nonChineseSendButton6_8->setText(texts.at(52));
    ui.nonChineseSendButton6_9->setText(texts.at(53));
}

uint qHash(const KMainWindow::NonChineseInputMode& key, uint seed) noexcept {
    return ::qHash(static_cast<int>(key), seed);
}

void KMainWindow::sendText(QString text) {
    //tsfManager->InsertTextAtCaret(reinterpret_cast<const wchar_t*>(text.utf16()));
    ui.scribblearea->clear();
}

void KMainWindow::updateButtonFontSize(int pattern) {
    static const QMap<int, int> fontSizeMap = []() {
        QMap<int, int> map;
        map.insert(2, 20);  // ��
        map.insert(1, 16);  // ��
        map.insert(0, 12);  // С
        return map;
    }();
    int fontSize = fontSizeMap.value(pattern);
    QList<SendTextButton*> sendTextButtons = ui.writingTab->findChildren<SendTextButton*>();
    auto setFontSize = [](QPushButton* button, int size) {
        QFont font = QApplication::font();
        font.setPointSize(size);
        button->setFont(font);
        };
    for (SendTextButton* button : sendTextButtons) {
        if (button != ui.resultTextButton_1) {
            setFontSize(button, fontSize);
        }
    }
    setFontSize(ui.resultTextButton_1, fontSize*2);
}

void KMainWindow::setDefaultPerferences() {
    ui.autoModeButton->clicked(); // Ĭ���Զ�ʶ��ģʽ
    ui.lineWidthSpinBox->setValue(4); // ����Ĭ�ϵ��߿�
    ui.blackColorButton->clicked();  // ����Ĭ�ϵıʼ���ɫΪ��ɫ
    ui.wordWidthComboBox->setCurrentIndex(0); // ����Ĭ�ϵ����ֿ��ΪС
}