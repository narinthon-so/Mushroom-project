Public Class Form1
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        'TODO: This line of code loads data into the 'MushroomDBDataSet.mushroom' table. You can move, or remove it, as needed.
        Me.MushroomTableAdapter.Fill(Me.MushroomDBDataSet.mushroom)

    End Sub
End Class