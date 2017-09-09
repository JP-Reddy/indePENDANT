class CreateLatlongs < ActiveRecord::Migration[5.1]
  def change
    create_table :latlongs do |t|
      t.string :imei
      t.float :latitude
      t.float :longitude

      t.timestamps
    end
  end
end
