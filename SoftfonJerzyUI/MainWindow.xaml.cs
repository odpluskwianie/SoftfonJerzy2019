using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SoftfonJerzyUI
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private SoftfonJerzyCore.SoftfonJerzyWrapper wrapper;
        private CallInfoUpdater callInfoUpdater;
        private RegInfoUpdater regInfoUpdater;
        private IfAnswerTheCallNotifier ifAnswerTheCallNotifier;
        
        public void updateCallInfo(String callInfo)
        {
            labelCallState.Content = callInfo;
        }
        public delegate void updateCallInfoDelegate(String callInfo);
        updateCallInfoDelegate updateCallInfoDelegateObject;

        public void updateRegInfo(String regInfo)
        {
            labelRegisterState.Content = regInfo;
        }
        public delegate void updateRegInfoDelegate(String callInfo);
        updateRegInfoDelegate updateRegInfoDelegateObject;
        /*
        public delegate void setRegInfoCallback(String newInfo);
        public void SetRegInfo(String newInfo)
        {
            labelRegisterState.Content = newInfo;
        }

        public delegate void setCallInfoCallback(String newInfo);
        public void SetCallInfo(String newInfo)
        {
            labelCallState.Content = newInfo;
        }

        public delegate bool ifAnswerTheCallCallback(String callInfo);
        public bool IfAnswerTheCall(String callInfo)
        {
            if (MessageBox.Show(callInfo, "Question", MessageBoxButton.YesNo, MessageBoxImage.Information) == MessageBoxResult.No)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        */
        class IfAnswerTheCallNotifier : SoftfonJerzyCore.StringChecker
        {
            public override bool check(string @string)
            {
                if (MessageBox.Show(@string, "Question", MessageBoxButton.YesNo, MessageBoxImage.Information) == MessageBoxResult.No)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        class CallInfoUpdater : SoftfonJerzyCore.StringConsumer
        {
            MainWindow mainWindow;
            public CallInfoUpdater(MainWindow mainWindow)
            {
                this.mainWindow = mainWindow;
            }
            public override void consume(string @string)
            {
                //mainWindow.labelCallState.Content = @string;
                mainWindow.Dispatcher.Invoke(mainWindow.updateCallInfoDelegateObject, @string);
            }
        }
        class RegInfoUpdater : SoftfonJerzyCore.StringConsumer
        {
            MainWindow mainWindow;
            public RegInfoUpdater(MainWindow mainWindow)
            {
                this.mainWindow = mainWindow;
            }
            public override void consume(string @string)
            {
                //mainWindow.labelCallState.Content = @string;
                mainWindow.Dispatcher.Invoke(mainWindow.updateRegInfoDelegateObject, @string);
            }
        }

        public MainWindow()
        {
            callInfoUpdater = new CallInfoUpdater(this);
            regInfoUpdater = new RegInfoUpdater(this);
            ifAnswerTheCallNotifier = new IfAnswerTheCallNotifier();
            wrapper = new SoftfonJerzyCore.SoftfonJerzyWrapper(callInfoUpdater,regInfoUpdater,ifAnswerTheCallNotifier);
            InitializeComponent();
            updateCallInfoDelegateObject = new updateCallInfoDelegate(updateCallInfo);
            updateRegInfoDelegateObject = new updateRegInfoDelegate(updateRegInfo);
        }

        ~MainWindow()
        {
            //wrapper.Dispose();
        }


        protected override void OnClosed(EventArgs e)
        {
            wrapper.Dispose();
            base.OnClosed(e);
        }

        private void Button1_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "1";
        }

        private void Button2_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "2";
        }

        private void Button3_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "3";
        }

        private void Button4_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "4";
        }

        private void Button5_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "5";
        }

        private void Button6_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "6";
        }

        private void Button7_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "7";
        }

        private void Button8_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "8";
        }

        private void Button9_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "9";
        }

        private void ButtonAsterisk_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "*";
        }

        private void Button0_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "0";
        }

        private void ButtonGrill_Click(object sender, RoutedEventArgs e)
        {
            textBoxURI.Text += "#";
        }

        private void ButtonCall_Click(object sender, RoutedEventArgs e)
        {
            labelCallState.Content = "";
            try
            {
                String remoteURIString = "sip:" + textBoxURI.Text;
                System.Text.RegularExpressions.Regex regSIPURI, reg200OK;
                regSIPURI = new System.Text.RegularExpressions.Regex(@"^[0-9]*$");
                reg200OK = new System.Text.RegularExpressions.Regex(@"200");
                bool matchSIPURI = regSIPURI.IsMatch(textBoxURI.Text);
                bool match200OK = reg200OK.IsMatch((string)labelRegisterState.Content);
                if (matchSIPURI && match200OK) 
                {
                    remoteURIString += "@"+regURI;
                }
                wrapper.call(remoteURIString);
                /*
                if (regSIPURI.IsMatch(remoteURIString))
                {
                    wrapper.call(remoteURIString);
                }
                else
                {
                    labelCallState.Content = "Podano błędny URI";
                }*/
            }
            catch(Exception)
            {
                labelCallState.Content = "Błąd połączenia. Sprawdź URI";
            }
        }

        private String userURI="", regURI="", userLogin="";


        private void ButtonRegister_Click(object sender, RoutedEventArgs e)
        {
            labelRegisterState.Content = "Trwa próba rejestracji...";
            try
            {
                wrapper.registerAccount("sip:" + textBoxUserURI.Text, "sip:" + textBoxRegURI.Text, textBoxLogin.Text, passwordBoxPassword.Password);
                userURI = textBoxUserURI.Text;
                regURI = textBoxRegURI.Text;
                userLogin = textBoxLogin.Text;
            }
            catch (Exception)
            {
                labelRegisterState.Content = "Błąd rejestracji. Sprawdź URI";
            }
        }

        private void ButtonHangUp_Click(object sender, RoutedEventArgs e)
        {
            wrapper.hangUpAllCalls();
        }
    }
}
