<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class DataLogger
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(DataLogger))
        Me.DataGridView1 = New System.Windows.Forms.DataGridView()
        Me.IDDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.TemperatureDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.HumidityDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.SavedateDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.SavetimeDataGridViewTextBoxColumn = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Dht22BindingSource = New System.Windows.Forms.BindingSource(Me.components)
        Me.MushroomDataSet = New WindowsApp2.MushroomDataSet()
        Me.Dht22TableAdapter = New WindowsApp2.MushroomDataSetTableAdapters.dht22TableAdapter()
        Me.DateTimePicker1 = New System.Windows.Forms.DateTimePicker()
        Me.LabelDataLoggerTitle = New System.Windows.Forms.Label()
        Me.ButtonReset = New System.Windows.Forms.Button()
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.Dht22BindingSource, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.MushroomDataSet, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'DataGridView1
        '
        Me.DataGridView1.AutoGenerateColumns = False
        Me.DataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridView1.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.IDDataGridViewTextBoxColumn, Me.TemperatureDataGridViewTextBoxColumn, Me.HumidityDataGridViewTextBoxColumn, Me.SavedateDataGridViewTextBoxColumn, Me.SavetimeDataGridViewTextBoxColumn})
        Me.DataGridView1.DataSource = Me.Dht22BindingSource
        Me.DataGridView1.Location = New System.Drawing.Point(12, 109)
        Me.DataGridView1.Name = "DataGridView1"
        Me.DataGridView1.RowHeadersWidth = 51
        Me.DataGridView1.RowTemplate.Height = 24
        Me.DataGridView1.Size = New System.Drawing.Size(958, 532)
        Me.DataGridView1.TabIndex = 0
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
        'Dht22BindingSource
        '
        Me.Dht22BindingSource.DataMember = "dht22"
        Me.Dht22BindingSource.DataSource = Me.MushroomDataSet
        '
        'MushroomDataSet
        '
        Me.MushroomDataSet.DataSetName = "MushroomDataSet"
        Me.MushroomDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema
        '
        'Dht22TableAdapter
        '
        Me.Dht22TableAdapter.ClearBeforeFill = True
        '
        'DateTimePicker1
        '
        Me.DateTimePicker1.Location = New System.Drawing.Point(351, 73)
        Me.DateTimePicker1.Name = "DateTimePicker1"
        Me.DateTimePicker1.Size = New System.Drawing.Size(200, 22)
        Me.DateTimePicker1.TabIndex = 1
        '
        'LabelDataLoggerTitle
        '
        Me.LabelDataLoggerTitle.AutoSize = True
        Me.LabelDataLoggerTitle.Font = New System.Drawing.Font("Microsoft Sans Serif", 19.8!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelDataLoggerTitle.Location = New System.Drawing.Point(302, 9)
        Me.LabelDataLoggerTitle.Name = "LabelDataLoggerTitle"
        Me.LabelDataLoggerTitle.Size = New System.Drawing.Size(379, 38)
        Me.LabelDataLoggerTitle.TabIndex = 2
        Me.LabelDataLoggerTitle.Text = "Mushroom Data Logger"
        '
        'ButtonReset
        '
        Me.ButtonReset.Location = New System.Drawing.Point(557, 73)
        Me.ButtonReset.Name = "ButtonReset"
        Me.ButtonReset.Size = New System.Drawing.Size(75, 23)
        Me.ButtonReset.TabIndex = 3
        Me.ButtonReset.Text = "Reset"
        Me.ButtonReset.UseVisualStyleBackColor = True
        '
        'DataLogger
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(982, 653)
        Me.Controls.Add(Me.ButtonReset)
        Me.Controls.Add(Me.LabelDataLoggerTitle)
        Me.Controls.Add(Me.DateTimePicker1)
        Me.Controls.Add(Me.DataGridView1)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "DataLogger"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Data Logger"
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.Dht22BindingSource, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.MushroomDataSet, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents DataGridView1 As DataGridView
    Friend WithEvents MushroomDataSet As MushroomDataSet
    Friend WithEvents Dht22BindingSource As BindingSource
    Friend WithEvents Dht22TableAdapter As MushroomDataSetTableAdapters.dht22TableAdapter
    Friend WithEvents IDDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents TemperatureDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents HumidityDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents SavedateDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents SavetimeDataGridViewTextBoxColumn As DataGridViewTextBoxColumn
    Friend WithEvents DateTimePicker1 As DateTimePicker
    Friend WithEvents LabelDataLoggerTitle As Label
    Friend WithEvents ButtonReset As Button
End Class
