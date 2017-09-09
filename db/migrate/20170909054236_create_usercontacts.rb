class CreateUsercontacts < ActiveRecord::Migration[5.1]
  def change
    create_table :usercontacts do |t|
      t.string :imei
      t.string :contacts, array: true,default: '{}' 

      t.timestamps
    end
  end
end
