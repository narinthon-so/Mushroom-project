<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.DataGridView1 = New System.Windows.Forms.DataGridView()
        Me.MushroomDBDataSet = New WindowsApp2.mushroomDBDataSet()
        Me.MushroomBindingSource = New System.Windows.Forms.BindingSource(Me.components)
        Me.MushroomTableAdapter = New WindowsApp2.mushroomDBDataSetTableAdapters.mushroomTableAdapter()
        Me.IDDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.TemperatureDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.HumidityDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.SavedateDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.SavetimeDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.MushroomDBDataSet, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.MushroomBindingSource, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'DataGridView1
        '
        Me.DataGridView1.AutoGenerateColumns = False
        Me.DataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridView1.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.IDDataGridViewTextBoxColumn, Me.TemperatureDataGridViewTextBoxColumn, Me.HumidityDataGridViewTextBoxColumn, Me.SavedateDataGridViewTextBoxColumn, Me.SavetimeDataGridViewTextBoxColumn})
        Me.DataGridView1.DataSource = Me.MushroomBindingSource
        Me.DataGridView1.Location = New System.Drawing.Point(12, 12)
        Me.DataGridView1.Name = "DataGridView1"
        Me.DataGridView1.RowHeadersWidth = 51
        Me.DataGridView1.RowTemplate.Height = 24
        Me.DataGridView1.Size = New System.Drawing.Size(776, 426)
        Me.DataGridView1.TabIndex = 0
        '
        'MushroomDBDataSet
        '
        Me.MushroomDBDataSet.DataSetName = "mushroomDBDataSet"
        Me.MushroomDBDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema
        '
        'MushroomBindingSource
        '
        Me.MushroomBindingSource.DataMember = "mushroom"
        Me.MushroomBindingSource.DataSource = Me.MushroomDBDataSet
        '
        'MushroomTableAdapter
        '
        Me.MushroomTableAdapter.ClearBeforeFill = True
        '
        'IDDataGridViewTextBoxColumn
        '
        Me.IDDataGridViewTextBoxColumn.DataPropertyName = "ID"
        Me.IDDataGridViewTextBoxColumn.HeaderText = "ID"
        Me.IDDataGridViewTextBoxColumn.MinimumWidth = 6
        Me.IDDataGridViewTextBoxColumn.Name = "IDDataGridViewTextBoxColumn"
        Me.IDDataGridViewTextBoxColumn.Width = 125
        '
        'TemperatureDataGridViewTextBoxColumn
        '
        Me.TemperatureDataGridViewTextBoxColumn.DataPropertyName = "temperature"
        Me.TemperatureDataGridViewTextBoxColumn.HeaderText = "temperature"
        Me.TemperatureDataGridViewTextBoxColumn.MinimumWidth = 6
        Me.TemperatureDataGridViewTextBoxColumn.Name = "TemperatureDataGridViewTextBoxColumn"
        Me.TemperatureDataGridViewTextBoxColumn.Width = 125
        '
        'HumidityDataGridViewTextBoxColumn
        '
        Me.HumidityDataGridViewTextBoxColumn.DataPropertyName = "humidity"
        Me.HumidityDataGridViewTextBoxColumn.HeaderText = "humidity"
        Me.HumidityDataGridViewTextBoxColumn.MinimumWidth = 6
        Me.HumidityDataGridViewTextBoxColumn.Name = "HumidityDataGridViewTextBoxColumn"
        Me.HumidityDataGridViewTextBoxColumn.Width = 125
        '
        'SavedateDataGridViewTextBoxColumn
        '
        Me.SavedateDataGridViewTextBoxColumn.DataPropertyName = "save_date"
        Me.SavedateDataGridViewTextBoxColumn.HeaderText = "save_date"
        Me.SavedateDataGridViewTextBoxColumn.MinimumWidth = 6
        Me.SavedateDataGridViewTextBoxColumn.Name = "SavedateDataGridViewTextBoxColumn"
        Me.SavedateDataGridViewTextBoxColumn.Width = 125
        '
        'SavetimeDataGridViewTextBoxColumn
        '
        Me.SavetimeDataGridViewTextBoxColumn.DataPropertyName = "save_time"
        Me.SavetimeDataGridViewTextBoxColumn.HeaderText = "save_time"
        Me.SavetimeDataGridViewTextBoxColumn.MinimumWidth = 6
        Me.SavetimeDataGridViewTextBoxColumn.Name = "SavetimeDataGridViewTextBoxColumn"
        Me.SavetimeDataGridViewTextBoxColumn.Width = 125
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 450)
        Me.Controls.Add(Me.DataGridView1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.MushroomDBDataSet, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.MushroomBindingSource, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents DataGridView1 As DataGridView
    Friend WithEvents MushroomDBDataSet As mushroomDBDataSet
    Friend WithEvents MushroomBindingSource As BindingSource
    Friend WithEvents MushroomTableAdapter As mushroomDBDataSetTableAdapters.mushroomTableAdapter
    Friend WithEvents IDDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents TemperatureDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents HumidityDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents SavedateDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents SavetimeDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
End Class
