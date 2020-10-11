Imports System.IO.Ports

Public Class Mushroom

    Dim temp As String
    Dim humi As String
    Dim set_temp_min As String
    Dim set_temp_max As String
    Dim set_humi_min As String
    Dim set_humi_max As String
    Dim ctrlMode As String
    Dim fanState As String
    Dim pumpState As String
    Dim lux As String
    Dim set_lux As String

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = False 'ทำให้เชื่อมต่ออุปกรณ์ภายนอกได้
        'SerialPort1.Open()
        For Each AvailableSerialPorts As String In SerialPort1.GetPortNames()

            ComboBox_AvailableSerialPorts.Items.Add(AvailableSerialPorts)
            ComboBox_AvailableSerialPorts.Text = AvailableSerialPorts

            SerialPort1.ReadTimeout = 2000

            Button_Connect.Visible = True
            Button_Disconnect.Visible = False

        Next

        Label8.Visible = False
        Label27.Visible = False
        Label9.Visible = False
        Label26.Visible = False
        Button1.Visible = False
        Button2.Visible = False
        Button6.Visible = False
        Button7.Visible = False

    End Sub

    Private Sub Label1_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Label2_Click(sender As Object, e As EventArgs) Handles Label2.Click

    End Sub

    Private Sub Label3_Click(sender As Object, e As EventArgs) Handles Label3.Click

    End Sub

    Private Sub Label4_Click(sender As Object, e As EventArgs) Handles Label4.Click

    End Sub

    Private Sub Label12_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        SerialPort1.Write("P")
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        SerialPort1.Write("F")
    End Sub

    Private Sub TextBox2_TextChanged(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button_Connect_Click(sender As Object, e As EventArgs) Handles Button_Connect.Click
        SerialPort1.PortName = ComboBox_AvailableSerialPorts.SelectedItem
        Button_Connect.Visible = False
        Button_Disconnect.Visible = True

        If SerialPort1.IsOpen = False Then
            SerialPort1.Open()
            Label15.Text = SerialPort1.BaudRate
        End If
    End Sub

    Private Sub Button_Disconnect_Click(sender As Object, e As EventArgs) Handles Button_Disconnect.Click
        If SerialPort1.IsOpen = True Then
            SerialPort1.Close()
            Label15.Text = "-"

            Label17.Text = "-" 'เมื่อ disconnect จะเช็ตค่าใหม่ ซ่อนสถานะ และ ปุ่มควบคุม
            Label18.Text = "-"
            Label21.Text = "-"
            Label22.Text = "-"
            Label25.Text = "-"
            Label36.Text = "-"
            Label37.Text = "-"
            LabelLux.Text = "-"
            LabelSetLux.Text = "-"
            Label8.Visible = False
            Label27.Visible = False
            Label9.Visible = False
            Label26.Visible = False
            Button1.Visible = False
            Button2.Visible = False
            Button6.Visible = False
            Button7.Visible = False

        End If

        Button_Connect.Visible = True
        Button_Disconnect.Visible = False
    End Sub

    Private Sub Label15_Click(sender As Object, e As EventArgs) Handles Label15.Click

    End Sub

    Private Sub SerialPort1_DataReceived(sender As Object, e As SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        Dim line As String = SerialPort1.ReadLine()  'serial value

        'Label16.Text = line                          'debug

        Try

            temp = line.Substring(1, 5)              'temp  ค่าอุณหภูมิ
            Label17.Text = temp                         'show temp

            humi = line.Substring(7, 5)              'humi  ค่าความชื้น
            Label18.Text = humi                         'show humi

            set_temp_max = line.Substring(12, 2)         'setTemp  ค่าอุณภูมิที่กำหมดส่งมาจาก ESP
            Label37.Text = set_temp_max

            set_humi_min = line.Substring(14, 2)         'setHumi  ค่าความชื้นที่ส่งมาจาก ESP
            Label22.Text = set_humi_min                      'show setHumi

            ctrlMode = line.Substring(17, 1)        'โหมดการทำงานของระบบ
            pumpState = line.Substring(19, 1)       'สถานะการทำงานของ pump
            fanState = line.Substring(21, 1)        'สถานะการทำงานของ fan

            set_temp_min = line.Substring(22, 2)
            Label21.Text = set_temp_min

            set_humi_max = line.Substring(24, 2)
            Label36.Text = set_humi_max

            Dim lux_raw = line.Substring(27, 7)
            Dim I = lux_raw.Substring(0, 1)
            Dim II = lux_raw.Substring(0, 2)
            Dim III = lux_raw.Substring(0, 3)
            If III = "000" Then
                lux = lux_raw.Substring(3, 4)   '1 3  2 5  3 4
            ElseIf II = "00" Then
                lux = lux_raw.Substring(2, 5)
            ElseIf I = "0" Then
                lux = lux_raw.Substring(1, 6)
            Else
                lux = lux_raw
            End If
            LabelLux.Text = lux + "  lx"

            Dim set_lux_raw = line.Substring(34, 3)
            Dim O = set_lux_raw.Substring(0, 1)
            If O = "0" Then
                set_lux = set_lux_raw.Substring(1, 2)
            Else
                set_lux = set_lux_raw
            End If
            LabelSetLux.Text = set_lux + "  lx"

        Catch ex As Exception
            'MessageBox.Show("Error, Serial data.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try



        '  Label23.Text = set_humi_max 'debug

        If ctrlMode = "0" Then        'Auto mode
            Label25.Text = "AUTO"

            Button1.Visible = False  'ซ่อนปุ่มควบคุมทั้งหมด
            Button2.Visible = False
            Button6.Visible = False
            Button7.Visible = False

            If pumpState = "0" Then
                Label9.Visible = True  'pump ไม่ทำงาน
                Label26.Visible = False
            Else
                Label9.Visible = False  'pump ทำงาน
                Label26.Visible = True
            End If

            If fanState = "0" Then
                Label8.Visible = False  'fan ไม่ทำงาน
                Label27.Visible = True
            Else
                Label8.Visible = True   'fan ทำงาน
                Label27.Visible = False
            End If

        ElseIf ctrlMode = "1" Then   'Manual mode
            Label25.Text = "MANUAL"

            If pumpState = "0" Then
                Button2.Visible = True   'pump ไม่ทำงาน
                Button7.Visible = False

                Label9.Visible = True
                Label26.Visible = False
            Else
                Button2.Visible = False  'pump ทำงาน
                Button7.Visible = True

                Label9.Visible = False
                Label26.Visible = True
            End If
            If fanState = "0" Then
                Button1.Visible = True   'fan ไม่ทำงาน
                Button6.Visible = False

                Label8.Visible = False
                Label27.Visible = True
            Else
                Button1.Visible = False   'fan ทำงาน
                Button6.Visible = True

                Label8.Visible = True
                Label27.Visible = False
            End If

        End If

    End Sub

    Private Sub Label16_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Label22_Click(sender As Object, e As EventArgs) Handles Label22.Click

    End Sub

    Private Sub Label23_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click

        Try
            SerialPort1.Write("M")
        Catch ex As Exception
            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try

    End Sub

    Private Sub GroupBox2_Enter(sender As Object, e As EventArgs) Handles GroupBox2.Enter

    End Sub

    Private Sub Button7_Click(sender As Object, e As EventArgs) Handles Button7.Click
        SerialPort1.Write("P")
    End Sub

    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        SerialPort1.Write("F")
    End Sub

    Private Sub TextBox1_TextChanged(sender As Object, e As EventArgs)

        'LabelDebug.Text = TextBox1.Text

    End Sub

    Private Sub TextBox1_KeyPress(sender As Object, e As KeyPressEventArgs)

        'number only
        If Asc(e.KeyChar) < 48 Or Asc(e.KeyChar) > 57 Then
            e.Handled = True
            MessageBox.Show("You can only input number.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End If

    End Sub

    Private Sub TextBox2_KeyPress(sender As Object, e As KeyPressEventArgs)

        'number only
        If Asc(e.KeyChar) < 48 Or Asc(e.KeyChar) > 57 Then
            e.Handled = True
            MessageBox.Show("You can only input number.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End If

    End Sub

    Private Sub LabelClearTemp_Click(sender As Object, e As EventArgs)
        TextBox1.Clear()
    End Sub

    Private Sub LabelClearHumi_Click(sender As Object, e As EventArgs)
        TextBox2.Clear()
    End Sub

    Private Sub DataLoggerToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles DataLoggerToolStripMenuItem.Click
        Process.Start(My.Settings.config_database_url + "/esp-data.php")
    End Sub

    Private Sub ChartToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ChartToolStripMenuItem.Click
        Process.Start(My.Settings.config_database_url + "/esp-chart.php")
    End Sub

    Private Sub PhpMyAdminToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles PhpMyAdminToolStripMenuItem.Click
        Process.Start(My.Settings.config_database_url + "/phpmyadmin")
    End Sub

    Private Sub CloseToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles CloseToolStripMenuItem.Click
        Me.Close()
    End Sub

    Private Sub WebServerToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles WebServerToolStripMenuItem.Click
        Process.Start(My.Settings.config_webserver_url)
    End Sub

    Private Sub SettingToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles SettingToolStripMenuItem.Click
        Dim f As New ConfigURL
        f.ShowDialog()
    End Sub

    Private Sub Label21_Click(sender As Object, e As EventArgs) Handles Label21.Click

    End Sub

    Private Sub Label31_Click(sender As Object, e As EventArgs)
        TextBox4.Clear()
    End Sub

    Private Sub Label30_Click(sender As Object, e As EventArgs)
        TextBox3.Clear()
    End Sub

    Private Sub Button9_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button8_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub TextBox4_TextChanged(sender As Object, e As EventArgs)

    End Sub

    Private Sub TextBox4_KeyPress(sender As Object, e As KeyPressEventArgs)
        'number only
        If Asc(e.KeyChar) < 48 Or Asc(e.KeyChar) > 57 Then
            e.Handled = True
            MessageBox.Show("You can only input number.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End If
    End Sub

    Private Sub TextBox3_TextChanged(sender As Object, e As EventArgs)

    End Sub

    Private Sub TextBox3_KeyPress(sender As Object, e As KeyPressEventArgs)
        'number only
        If Asc(e.KeyChar) < 48 Or Asc(e.KeyChar) > 57 Then
            e.Handled = True
            MessageBox.Show("You can only input number.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End If
    End Sub

    Private Sub Button4_Click_1(sender As Object, e As EventArgs) Handles Button4.Click

        If SerialPort1.IsOpen = True Then

            If TextBox2.TextLength = 2 Then

                SerialPort1.Write("H" + TextBox2.Text)
                TextBox2.Clear()

            Else

                MessageBox.Show("The value should be between 10-99%.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

            End If

        ElseIf SerialPort1.IsOpen = False Then

            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        End If

    End Sub

    Private Sub Button3_Click_1(sender As Object, e As EventArgs) Handles Button3.Click

        If SerialPort1.IsOpen = True Then

            If TextBox1.TextLength = 2 Then

                SerialPort1.Write("Y" + TextBox1.Text)
                TextBox1.Clear()

            Else

                MessageBox.Show("The value should be between 10-99°C.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

            End If

        ElseIf SerialPort1.IsOpen = False Then

            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        End If

    End Sub

    Private Sub Button9_Click_1(sender As Object, e As EventArgs) Handles Button9.Click
        If SerialPort1.IsOpen = True Then

            If TextBox4.TextLength = 2 Then

                SerialPort1.Write("T" + TextBox4.Text)
                TextBox4.Clear()

            Else

                MessageBox.Show("The value should be between 10-99°C.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

            End If

        ElseIf SerialPort1.IsOpen = False Then

            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        End If
    End Sub

    Private Sub Button8_Click_1(sender As Object, e As EventArgs) Handles Button8.Click
        If SerialPort1.IsOpen = True Then

            If TextBox3.TextLength = 2 Then

                SerialPort1.Write("J" + TextBox3.Text)
                TextBox3.Clear()

            Else

                MessageBox.Show("The value should be between 10-99°C.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

            End If

        ElseIf SerialPort1.IsOpen = False Then

            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        End If
    End Sub

    Private Sub LogoutToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles LogoutToolStripMenuItem.Click
        Login.Show()
        Me.Close()
    End Sub

    Private Sub Label16_Click_1(sender As Object, e As EventArgs) Handles Label16.Click

    End Sub

    Private Sub LabelClearSetLux_Click(sender As Object, e As EventArgs) Handles LabelClearSetLux.Click
        TextBoxSetLux.Clear()
    End Sub

    Private Sub ButtonSetLux_Click(sender As Object, e As EventArgs) Handles ButtonSetLux.Click
        If SerialPort1.IsOpen = True Then

            If TextBoxSetLux.TextLength >= 2 Then

                SerialPort1.Write("L" + TextBoxSetLux.Text)
                TextBoxSetLux.Clear()

            Else

                MessageBox.Show("The value should be between 10-999 lx.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

            End If

        ElseIf SerialPort1.IsOpen = False Then

            MessageBox.Show("The serial port is closed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

        End If
    End Sub
End Class
