# Generated by Django 2.1.3 on 2018-12-02 18:53

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('products', '0007_auto_20181202_1850'),
    ]

    operations = [
        migrations.AlterField(
            model_name='accessory',
            name='product',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, primary_key=True, serialize=False, to='products.Products'),
        ),
        migrations.AlterField(
            model_name='costumes',
            name='product',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, primary_key=True, serialize=False, to='products.Products'),
        ),
    ]
