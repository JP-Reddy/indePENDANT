class CreateUsers < ActiveRecord::Migration[5.1]
  def change
    create_table :users do |t|
      t.string :imei
      # t.string :contact, array:true, default: []
      t.float :latitude
      t.float :longitude
      t.timestamps
    end
  end
end
