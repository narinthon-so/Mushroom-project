Public Class Login
    Private Sub Login_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If TextBox1.Text = "admin" And TextBox2.Text = "admin" Then
            Mushroom.Show()
            Me.Hide()
            TextBox1.Clear()
            TextBox2.Clear()
        Else
            MsgBox("Sorry, The Username or Password was incorrect.", MsgBoxStyle.Critical, "Error")
        End If
    End Sub
End Class