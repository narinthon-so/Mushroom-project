Imports System.IO.Ports

Public Class Mushroom
    Dim temp As String
    Dim humi As String
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

    Private Sub Label1_Click(sender As Object, e As EventArgs) Handles Label1.Click

    End Sub

    Private Sub Label2_Click(sender As Object, e As EventArgs) Handles Label2.Click

    End Sub

    Private Sub Label3_Click(sender As Object, e As EventArgs) Handles Label3.Click

    End Sub

    Private Sub Label4_Click(sender As Object, e As EventArgs) Handles Label4.Click

    End Sub

    Private Sub Label12_Click(sender As Object, e As EventArgs) Handles Label12.Click

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        SerialPort1.Write("P1")
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        SerialPort1.Write("F1")
    End Sub

    Private Sub TextBox2_TextChanged(sender As Object, e As EventArgs) Handles TextBox2.TextChanged

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

        Label16.Text = line                          'debug

        temp = line.Substring(1, 5)              'temp  ค่าอุณหภูมิ
        Label17.Text = temp                         'show temp
        humi = line.Substring(7, 5)              'humi  ค่าความชื้น
        Label18.Text = humi                         'show humi

        Dim setTemp = line.Substring(12, 2)         'setTemp  ค่าอุณภูมิที่กำหมดส่งมาจาก ESP
        Label21.Text = setTemp                      'show setTemp
        Dim setHumi = line.Substring(14, 2)         'setHumi  ค่าความชื้นที่ส่งมาจาก ESP
        Label22.Text = setHumi                      'show setHumi
        Dim ctrlMode = line.Substring(17, 1)        'โหมดการทำงานของระบบ
        Dim pumpState = line.Substring(19, 1)       'สถานะการทำงานของ pump
        Dim fanState = line.Substring(21, 1)        'สถานะการทำงานของ fan

        Label23.Text = line.Length() 'debug

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

    Private Sub Label16_Click(sender As Object, e As EventArgs) Handles Label16.Click

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click

        SerialPort1.Write("T" + TextBox1.Text)
        TextBox1.Clear()

    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click

        SerialPort1.Write("H" + TextBox2.Text)
        TextBox2.Clear()


    End Sub

    Private Sub Label22_Click(sender As Object, e As EventArgs) Handles Label22.Click

    End Sub

    Private Sub Label23_Click(sender As Object, e As EventArgs) Handles Label23.Click

    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        SerialPort1.Write("M")
    End Sub

    Private Sub GroupBox2_Enter(sender As Object, e As EventArgs) Handles GroupBox2.Enter

    End Sub

    Private Sub Button7_Click(sender As Object, e As EventArgs) Handles Button7.Click
        SerialPort1.Write("P0")
    End Sub

    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        SerialPort1.Write("F0")
    End Sub

End Class
