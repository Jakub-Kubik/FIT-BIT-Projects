# Generated by Django 2.1.3 on 2018-12-02 18:47

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('products', '0005_auto_20181202_1821'),
    ]

    operations = [
        migrations.RenameField(
            model_name='accessory',
            old_name='number',
            new_name='amount',
        ),
        migrations.RenameField(
            model_name='costumes',
            old_name='number',
            new_name='amount',
        ),
    ]